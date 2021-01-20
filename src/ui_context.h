# pragma once

#include <functional>
#include <iostream>
#include <map>
#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <redux.hpp>

#include "renderers/renderer.h"

struct State {
  std::string world_seed;
  int world_size;
  // FIXME If I don't want to copy state all the time (which seems dangerous),
  // I need to update redux.hpp to use references where appropriate.
  // State(const State&) = delete; // Delete copy constructor
  // State& operator=(const State&) = delete; // Delete copy assignment
};

class Action {
  public:
    enum class Type {
      CREATE_NEW_WORLD
    };

  public:
    virtual ~Action() {
      std::cout << "Action destroyed" << std::endl;
    };
    Action(const Action&) = delete; // Delete copy constructor
    Action& operator=(const Action&) = delete; // Delete copy assignment

    const Type getType() const {
      return m_type;
    };

  protected:
    Action(Type type) : m_type{type} {};

  private:
    const Type m_type;
};

class CreateNewWorldAction : public Action {
  public:
    CreateNewWorldAction(std::string seed) : Action { Type::CREATE_NEW_WORLD }, m_seed{seed} {};
    std::string getSeed() { return m_seed; };
  private:
    std::string m_seed;
};

static State theReducer(State state, Action* action) {
  switch(action->getType()) {
    case Action::Type::CREATE_NEW_WORLD:
      std::cout << "In CREATE_NEW_WORLD action handler" << std::endl;
      state.world_seed = dynamic_cast<CreateNewWorldAction*>(action)->getSeed();
      state.world_size = 2;
      break;
  }

  // Because we need an Action* to dynamic_cast, when dispatch is called, we
  // pass it a new WhateverAction() which means we need to manually delete it
  // when done.
  delete action;

  return state;
};

using Store = redux::Store<State, Action*>;

class UIContext {
  private:
    GLFWwindow* m_window;

  public:
    UIContext(GLFWwindow* window);

    //
    // Well, I'm going to try to use redux again.
    // I'm just putting it here because it's the easiest way to be
    // able to access it since most places have a ui context.
    //
  private:
    State m_my_state {};
    Store m_store { Store(theReducer, m_my_state) };

  public:
    Store& getStore() { return m_store; };

    //
    // Quit functionality
    //
  public:
    void quit();

    //
    // Page functionality
    // UIContext keeps track of the current "page" to be rendered. It's
    // basically the top level node for e.g. the intro menu, the new world
    // form, the game world.
    // In the code I just use Renderer and the naming is all over the place.
    // RootRenderer is basically the game renderer. MainMenuRenderer is
    // probably better referred to as the the IntroMenuRenderer.
    //
  private:
    std::shared_ptr<Renderer> m_current_page_ptr {};
  public:
    std::shared_ptr<Renderer> getCurrentPagePtr();
    void changeCurrentPage(std::shared_ptr<Renderer> renderer_ptr);

    //
    // Window size accessors.
    // Gives consumers the ability to get the width and height each frame
    // without having to query glfw or deal with a callback and track the
    // values itself. UIContext uses the callback to keep them up-to-date.
    //
  private:
    int m_width;
    int m_height;
  public:
    void windowSizeCallback(int width, int height);
    int getWidth() const;
    int getHeight() const;

    //
    // General purpose handler functionality
    //
  public:
    void removeHandlers(void* instance);

    //
    // Per-frame Input processing
    //
  private:
    std::map<void*, std::function<void(double)>> m_process_input_funcs;
    void removeProcessInput(void* instance);
  public:
    void processInput(double dt);
    void addProcessInput(void* instance, std::function<void(double)> f);

    //
    // Cursor Moved Callback
    //
  private:
    std::map<void*, std::function<void(double,double)>> m_cursor_moved_handlers;
    void removeCursorMovedHandler(void* instance);
  public:
    // Called by window
    void cursorPosCallback(double xpos, double ypos);

    // Called by consumers
    void addCursorMovedHandler(void* instance, std::function<void(double, double)> f);

    //
    // Key Callback
    //
  private:
    std::map<int, std::map<void*,std::function<void()>>> m_handlers_by_key_map;
    void removeKeyPressedHandlers(void* instance);

  public:
    // Called by Window
    void keyCallback(int key, int scancode, int action, int mods);

    // Called by consumers of UIContext
    void addKeyPressedHandler(int key, void* instance, std::function<void()> f);

    //
    // Key State
    //
  public:
    bool isKeyPressed(int key);

    //
    // Game/Gui Focus
    //
  private:
    std::map<void*, std::function<void(bool)>> m_game_gui_focus_changed_handlers;
    void removeGameGuiFocusChangedHandler(void* instance);
  public:
    void focusInGame();
    void focusInGUI();
    bool isFocusedInGame() const;
    bool isFocusedInGUI() const;
    void addGameGuiFocusChangedHandler(void* instance, std::function<void(bool)>);
};
