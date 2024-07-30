#pragma once

#include <godot_cpp/classes/node.hpp>

#include <memory>
#include <vector>
#include <map>

#include "../wrappers/game-wrapper/src/game-wrapper.h"

namespace godot_scgms {

	/*
	 * Game wrapper class for Godot engine
	 * This class is indended to be used as a singleton, though an ordinary use is possible as well upon enhancing register_types
	 */
	class GDGameWrapper : public godot::Node {
		GDCLASS(GDGameWrapper, godot::Node)

		private:
			// helper structure for scheduling signals
			struct TScheduled_Signal {
				GUID id;		// signal ID (we usually want to take this from scgms-common)
				double time;	// relative time fraction
				double level;	// signal level
			};

			// last assigned instance ID
			int mLast_Game_Instance_Id = 0;
			// map of game instances; may become sparse, hence the map
			std::map<int, std::unique_ptr<CGame_Wrapper>> mGame_Instances;
			// map of vectors of scheduled signals per game instance
			std::map<int, std::vector<TScheduled_Signal>> mGame_Scheduled_Signals;
			// map of game states
			std::map<int, CPatient_Sensor_State> mGame_States;

		protected:
			// godot object contract - bind methods of the class
			static void _bind_methods() {
				godot::ClassDB::bind_method(godot::D_METHOD("Create"), &GDGameWrapper::Create);
				godot::ClassDB::bind_method(godot::D_METHOD("ScheduleInsulinBolus"), &GDGameWrapper::ScheduleInsulinBolus);
				godot::ClassDB::bind_method(godot::D_METHOD("ScheduleInsulinBasalRate"), &GDGameWrapper::ScheduleInsulinBasalRate);
				godot::ClassDB::bind_method(godot::D_METHOD("ScheduleCarbohydratesIntake"), &GDGameWrapper::ScheduleCarbohydratesIntake);
				godot::ClassDB::bind_method(godot::D_METHOD("ScheduleCarbohydratesRescue"), &GDGameWrapper::ScheduleCarbohydratesRescue);
				godot::ClassDB::bind_method(godot::D_METHOD("SchedulePhysicalActivity"), &GDGameWrapper::SchedulePhysicalActivity);

				godot::ClassDB::bind_method(godot::D_METHOD("Get_BG"), &GDGameWrapper::Get_BG);
				godot::ClassDB::bind_method(godot::D_METHOD("Get_IG"), &GDGameWrapper::Get_IG);
				godot::ClassDB::bind_method(godot::D_METHOD("Get_IOB"), &GDGameWrapper::Get_IOB);
				godot::ClassDB::bind_method(godot::D_METHOD("Get_COB"), &GDGameWrapper::Get_COB);

				godot::ClassDB::bind_method(godot::D_METHOD("Step"), &GDGameWrapper::Step);
				godot::ClassDB::bind_method(godot::D_METHOD("Terminate"), &GDGameWrapper::Terminate);
			}

		public:
			GDGameWrapper();
			~GDGameWrapper();

			// creates the game instance with given parameters; returns newly created game ID
			int Create(int16_t configClass, int16_t configId, uint32_t steppingMs, godot::String logFilePath);
			
			// schedules insulin bolus of given amount at given time-step fraction
			bool ScheduleInsulinBolus(int gameId, double level, double time);
			// schedules insulin basal rate change of given amount at given time-step fraction
			bool ScheduleInsulinBasalRate(int gameId, double level, double time);
			// schedules carbohydrate intake of given amount at given time-step fraction
			bool ScheduleCarbohydratesIntake(int gameId, double level, double time);
			// schedules rescue carbohydrate intake of given amount at given time-step fraction
			bool ScheduleCarbohydratesRescue(int gameId, double level, double time);
			// schedules physical activity of given amount at given time-step fraction
			bool SchedulePhysicalActivity(int gameId, double level, double time);

			// retrieves the current blood glucose
			double Get_BG(int gameId);
			// retrieves the current interstitial glucose
			double Get_IG(int gameId);
			// retrieves the current insulin on board
			double Get_IOB(int gameId);
			// retrieves the current carbohydrates on board
			double Get_COB(int gameId);

			// steps the given game by a pre-configured time amount
			bool Step(int gameId);
			// terminates the given game, invalidating the gameId
			bool Terminate(int gameId);
	};

}
