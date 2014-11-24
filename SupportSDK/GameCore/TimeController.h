#pragma once

class TickListener
  {
  public:
    virtual void TickPerformed(long i_ms_for_tick) = 0;
  };

class TimeController
  {
  private:
    //list of listeners that will handle message that tick is performed
    std::vector<TickListener*> m_listeners;

  public:
    TimeController();
    ~TimeController();

    void Update(long i_elapsed_time);
    void AddSubscriber(TickListener* ip_listener);
    void RemoveSubscriber(TickListener* ip_listener);
  };