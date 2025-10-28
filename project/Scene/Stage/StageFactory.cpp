#include "StageFactory.h"
#include "Stage1Logic.h"
#include "Stage2Logic.h"

std::unique_ptr<IStageLogic> CreateStageLogic(StageId id) {
	switch (id) {
	case StageId::Stage1:
		return std::make_unique<Stage1Logic>();
	case StageId::Stage2:
		return std::make_unique<Stage2Logic>();

	default: return std::make_unique<Stage1Logic>();
	}
}
