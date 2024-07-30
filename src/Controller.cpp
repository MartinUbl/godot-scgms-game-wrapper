#include "Controller.h"

// base namespace for godot_scgms wrapper
namespace godot_scgms {

	GDGameWrapper::GDGameWrapper() {
		//
	}

	GDGameWrapper::~GDGameWrapper() {
		//
	}

	int GDGameWrapper::Create(int16_t configClass, int16_t configId, uint32_t steppingMs, godot::String logFilePath) {

		// create game wrapper with given time step
		std::unique_ptr<CGame_Wrapper> wrapper = std::make_unique<CGame_Wrapper>(steppingMs);
		if (!wrapper) {
			return -1;
		}

		if (!wrapper->Load_Configuration(configClass, configId, logFilePath.utf8().get_data()))
			return -1;

		if (!wrapper->Execute_Configuration())
			return -1;

		// initial step - this initializes the embedded model and forces it to emit current state
		wrapper->Step(true);

		// assign game ID
		int assignedId = mLast_Game_Instance_Id;
		mLast_Game_Instance_Id++;

		mGame_Scheduled_Signals[assignedId].clear();
		// store current state
		mGame_States[assignedId] = wrapper->Get_State();

		mGame_Instances.insert({ assignedId, std::move(wrapper) });

		return assignedId;
	}

	bool GDGameWrapper::ScheduleInsulinBolus(int gameId, double level, double time) {

		if (mGame_Instances.find(gameId) == mGame_Instances.end()) {
			return false;
		}

		mGame_Scheduled_Signals[gameId].push_back({
			scgms::signal_Requested_Insulin_Bolus,
			time,
			level
		});

		return true;
	}

	bool GDGameWrapper::ScheduleInsulinBasalRate(int gameId, double level, double time) {

		if (mGame_Instances.find(gameId) == mGame_Instances.end()) {
			return false;
		}

		mGame_Scheduled_Signals[gameId].push_back({
			scgms::signal_Requested_Insulin_Basal_Rate,
			time,
			level
		});

		return true;
	}

	bool GDGameWrapper::ScheduleCarbohydratesIntake(int gameId, double level, double time) {

		if (mGame_Instances.find(gameId) == mGame_Instances.end()) {
			return false;
		}

		mGame_Scheduled_Signals[gameId].push_back({
			scgms::signal_Carb_Intake,
			time,
			level
		});

		return true;
	}

	bool GDGameWrapper::ScheduleCarbohydratesRescue(int gameId, double level, double time) {

		if (mGame_Instances.find(gameId) == mGame_Instances.end()) {
			return false;
		}

		mGame_Scheduled_Signals[gameId].push_back({
			scgms::signal_Carb_Rescue,
			time,
			level
		});

		return true;
	}

	bool GDGameWrapper::SchedulePhysicalActivity(int gameId, double level, double time) {

		if (mGame_Instances.find(gameId) == mGame_Instances.end()) {
			return false;
		}

		mGame_Scheduled_Signals[gameId].push_back({
			scgms::signal_Physical_Activity,
			time,
			level
		});

		return true;
	}

	double GDGameWrapper::Get_BG(int gameId) {

		if (mGame_States.find(gameId) == mGame_States.end()) {
			return std::numeric_limits<double>::quiet_NaN();
		}

		return mGame_States[gameId].bg;
	}

	double GDGameWrapper::Get_IG(int gameId) {
		if (mGame_States.find(gameId) == mGame_States.end()) {
			return std::numeric_limits<double>::quiet_NaN();
		}

		return mGame_States[gameId].ig;
	}

	double GDGameWrapper::Get_IOB(int gameId) {
		if (mGame_States.find(gameId) == mGame_States.end()) {
			return std::numeric_limits<double>::quiet_NaN();
		}

		return mGame_States[gameId].iob;
	}

	double GDGameWrapper::Get_COB(int gameId) {
		if (mGame_States.find(gameId) == mGame_States.end()) {
			return std::numeric_limits<double>::quiet_NaN();
		}

		return mGame_States[gameId].cob;
	}

	bool GDGameWrapper::Step(int gameId) {

		if (mGame_Instances.find(gameId) == mGame_Instances.end()) {
			return false;
		}

		// sort scheduled signals to properly step the model
		// every discrete model steps only forward in time once a new value reaches its Execute method
		// this forces us to strictly order signals sent to the chain to avoid mistakenly discarding "older" events
		auto& vec = mGame_Scheduled_Signals[gameId];
		std::sort(vec.begin(), vec.end(), [](const TScheduled_Signal& a, const TScheduled_Signal& b) {
			return a.time < b.time;
		});

		auto& wrapper = mGame_Instances[gameId];

		// inject all scheduled signal levels
		for (auto& signal : vec) {
			if (!wrapper->Inject_Level(&signal.id, signal.level, signal.time)) {
				return false;
			}
		}

		mGame_Scheduled_Signals[gameId].clear();

		// step the model - this updates state
		if (!wrapper->Step()) {
			return false;
		}

		// update state cache on our side
		mGame_States[gameId] = wrapper->Get_State();

		return true;
	}

	bool GDGameWrapper::Terminate(int gameId) {
		if (mGame_Instances.find(gameId) == mGame_Instances.end() || !mGame_Instances[gameId]) {
			return false;
		}

		// terminates the chain, sends Shut_Down event and waits until it reaches the terminal filter
		mGame_Instances[gameId]->Terminate(TRUE);

		mGame_Scheduled_Signals.erase(gameId);
		mGame_States.erase(gameId);
		mGame_Instances.erase(gameId);

		return true;
	}

}
