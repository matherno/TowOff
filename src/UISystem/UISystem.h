#pragma once
//
// Created by matt on 27/12/17.
//


#include <mathernogl/MathernoGL.h>
#include <GameSystem/GameSystem.h>

class GameContext;

enum Alignment
  {
  alignmentStart,
  alignmentCentre,
  alignmentEnd,
  };

class UIComponent;
typedef std::shared_ptr<UIComponent> UIComponentPtr;
typedef std::function<bool(uint mouseX, uint mouseY)> OnMouseClickCallback;

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
  virtual Vector2D getOffset() const = 0;
  virtual Vector2D getSize() const = 0;
  virtual void setWidthMatchParent(bool match) = 0;
  virtual void setHeightMatchParent(bool match) = 0;
  virtual void setHorizontalAlignment(Alignment alignment) = 0;
  virtual void setVerticalAlignment(Alignment alignment) = 0;
  virtual void addChild(UIComponentPtr component) = 0;
  virtual void removeChild(uint id) = 0;
  virtual UIComponentPtr getChild(uint id) = 0;
  virtual void setVisible(bool visible, bool recurseChildren = false) = 0;
  virtual bool isVisible() const = 0;
  virtual bool mouseClick(GameContext* context, uint mouseX, uint mouseY) = 0;
  virtual bool hitTest(uint mouseX, uint mouseY, bool testChildren = false) = 0;
  virtual void setMouseClickCallback(OnMouseClickCallback func) = 0;
  virtual void setPadding(float horizPadding, float vertPadding) = 0;     // padding only used when matching parent size
  };

class UIManager
  {
public:
  virtual void addComponent(UIComponentPtr component) = 0;
  virtual void removeComponent(uint id) = 0;
  virtual UIComponentPtr getComponent(uint id) = 0;
  virtual uint getNextComponentID() = 0;
  virtual void enableModalMode(UIComponentPtr modalComponent) = 0;
  virtual void disableModalMode() = 0;
  virtual bool isModalModeActive() const = 0;
  virtual bool initialise(GameContext* context) = 0;
  virtual void update(GameContext* context) = 0;
  virtual void cleanUp(GameContext* context) = 0;
  virtual bool mouseClick(GameContext* context, uint mouseX, uint mouseY) = 0;
  virtual bool hitTest(uint mouseX, uint mouseY) = 0;
  };