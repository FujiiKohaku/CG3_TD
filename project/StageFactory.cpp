#include "StageFactory.h"
#include "Stage1Logic.h"

std::unique_ptr<IStageLogic> CreateStageLogic(StageId id) {
	switch (id) {
	case StageId::Stage1:
		return std::make_unique<Stage1Logic>();

	default: return std::make_unique<Stage1Logic>();
	}
}
