#pragma once

#include "UISystem.h"
#include "UIRenderable.h"

/*
*   
*/


class UIPanel : public UIComponent
  {
private:
  std::shared_ptr<UIRenderable> renderable;
  Vector2D offset;
  Vector2D size;
  bool widthMatchParent = false;
  bool heightMatchParent = false;
  Alignment horizAlignment = alignmentStart;
  Alignment vertAlignment = alignmentStart;
  Vector3D colour;
  mathernogl::MappedList<UIComponentPtr> children;
  std::list<UIComponentPtr> childrenToAdd;
  std::list<uint> childrenToRemove;
  Vector2D currentScreenPos, currentScreenSize;
  bool visible = true;
  OnMouseClickCallback mouseClickCallback;

protected:
  bool isValid = false;

public:
  UIPanel(uint id);

  virtual void initialise(GameContext* context) override;
  virtual void onUpdate(GameContext* context) override;
  virtual void cleanUp(GameContext* context) override;
  virtual void refresh(GameContext* context, const Vector2D& parentPos, const Vector2D& parentSize) override;
  virtual bool needsRefresh() override;
  virtual void invalidate() override;
  virtual void addChild(UIComponentPtr component) override;
  virtual void removeChild(uint id) override;
  virtual bool mouseClick(GameContext* context, uint mouseX, uint mouseY) override;
  virtual bool hitTest(uint mouseX, uint mouseY, bool testChildren = false) override;

  virtual void setBackgroundColour(const Vector3D& colour) { this->colour = colour; } ;
  virtual void setOffset(const Vector2D& offset) override { this->offset = offset; }
  virtual void setSize(const Vector2D& size) override { this->size = size; }
  virtual void setWidthMatchParent(bool match) override { widthMatchParent = match; }
  virtual void setHeightMatchParent(bool match) override { heightMatchParent = match; }
  virtual void setHorizontalAlignment(Alignment alignment) override { horizAlignment = alignment; }
  virtual void setVerticalAlignment(Alignment alignment) override { vertAlignment = alignment; }
  virtual void setVisible(bool visible) override;
  virtual void setMouseClickCallback(OnMouseClickCallback func) override { mouseClickCallback = func; }

private:
  void addPendingComponents(GameContext* context);
  void removePendingComponents(GameContext* context);
  void refreshPendingComponents(GameContext* context);

  };
