#pragma once
#include <memory>
#include "StageId.h"
#include "StageLogic.h"

std::unique_ptr<IStageLogic> CreateStageLogic(StageId id);