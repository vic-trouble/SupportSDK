#pragma once

class GameContext;

class IController
  {
  private:
    int           m_mask;
  protected:
    GameContext&  m_game_context;

  public:
    IController (int i_mask, GameContext& i_game_context)
      : m_mask(i_mask)
      , m_game_context(i_game_context)
      {      }
    virtual ~IController() { }

    int             GetMask() const;
    GameContext&    GetContext();
  };

//////////////////////////////////////////////////////////////////////////

inline int IController::GetMask() const
  {
  return m_mask;
  }

inline GameContext& IController::GetContext()
  {
  return m_game_context;
  }