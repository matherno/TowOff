#pragma once
//
// Created by matt on 27/12/17.
//


#include <mathernogl/MathernoGL.h>
#include <src/GameSystem/GameSystem.h>

class GameContext;

enum Alignment
  {
  alignmentStart,
  alignmentCentre,
  alignmentEnd,
  };

class UIComponent;
typedef std::shared_ptr<UIComponent> UIComponentPtr;

class UIComponent
  {
private:
  const uint id;
public:
  UIComponent(uint id) : id(id) {}
  uint getID(){ return id; }
  virtual void initialise(GameContext* context) = 0;
  virtual void onUpdate(GameContext* context) = 0;
  virtual void cleanUp(GameContext* context) = 0;
  virtual void refresh(GameContext* context, const Vector2D& parentPos, const Vector2D& parentSize) = 0;
  virtual bool needsRefresh() = 0;
  virtual void invalidate() = 0;
  virtual void setOffset(const Vector2D& offset) = 0;
  virtual void setSize(const Vector2D& size) = 0;
  virtual void setWidthMatchParent(bool match) = 0;
  virtual void setHeightMatchParent(bool match) = 0;
  virtual void setHorizontalAlignment(Alignment alignment) = 0;
  virtual void setVerticalAlignment(Alignment alignment) = 0;
  virtual void addChild(UIComponentPtr component) = 0;
  virtual void removeChild(uint id) = 0;
  virtual void setVisible(bool visible) = 0;
  };

class UIManager
  {
public:
  virtual void addComponent(UIComponentPtr component) = 0;
  virtual void removeComponent(uint id) = 0;
  virtual uint getNextComponentID() = 0;
  virtual bool initialise(GameContext* context) = 0;
  virtual void update(GameContext* context) = 0;
  virtual void cleanUp(GameContext* context) = 0;
  };