#pragma once

#include <Math/Vector.h>

class IMovable
  {
  protected:
    SDK::Math::Vector<SDK::real, 2> m_position;

  public:
    virtual   ~IMovable(){}

    void      SetPosition(const SDK::Math::Vector<float, 2>& i_position)
      {
      m_position = i_position;
      }

    SDK::Math::Vector<SDK::real, 2>  GetPosition() const
      {
      return m_position;
      }

    SDK::Math::Vector<SDK::real, 2>& AccessPosition()
      {
      return m_position;
      }
  };