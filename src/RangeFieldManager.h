#pragma once

#include <GameSystem/GameSystem.h>
#include <RenderSystem/RenderableLineStrips.h>

/*
*   
*/

class RangeFieldManager : public GameActor
  {
private:
  typedef std::vector<uint> LineStripIDs;
  mathernogl::MappedList<LineStripIDs> rangeFieldLineStrips;
  uint nextRangeFieldID = 0;
  std::shared_ptr<RenderableLineStrips> lineStripsRenderable;
  float transparency = 0.8f;

public:
  RangeFieldManager(uint id);

  virtual void onAttached(GameContext* gameContext) override;
  virtual void onUpdate(GameContext* gameContext) override;
  virtual void onDetached(GameContext* gameContext) override;

  uint createRangeField(const Vector3D& position, float radius, const Vector3D& colour);
  void removeRangeField(uint id);
  };
