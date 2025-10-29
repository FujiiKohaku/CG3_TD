#include "StageFactory.h"
#include "Stage1Logic.h"
#include "Stage2Logic.h"
#include "Stage3Logic.h"

std::unique_ptr<IStageLogic> CreateStageLogic(StageId id) {
	switch (id) {
	case StageId::Stage1:
		return std::make_unique<Stage1Logic>();
	case StageId::Stage2:
		return std::make_unique<Stage2Logic>();
	case StageId::Stage3:
		return std::make_unique<Stage3Logic>();

	default: return std::make_unique<Stage1Logic>();
	}
}
