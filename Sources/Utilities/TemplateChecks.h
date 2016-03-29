#ifndef __TEMPLATECHECKS_H__
#define __TEMPLATECHECKS_H__

namespace SDK
  {

  template <typename Base, typename Derived>
  struct DerivedFrom
    {
    static void constraints(Derived* ip_derived) { Base* p_base = ip_derived; }
    DerivedFrom() { void(*p)(Derived*) = constraints; }
    };

  } // SDK

#endif