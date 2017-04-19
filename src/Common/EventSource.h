//
// Created by Kane York on 4/18/17.
//

#ifndef UITEST_EVENTSOURCE_H
#define UITEST_EVENTSOURCE_H

#include <functional>
#include <vector>

template <typename Evt> class EventSource<Evt> {
  public:
	typedef std::function<void, Evt> Func;
	virtual NotifyHandle<Evt> cb_register(Func callback) = 0;
	virtual void cb_unregister(int private_id) = 0;
};

template <typename Evt> class NotifyHandle<Evt> {
  public:
	typedef std::function<void, Evt> Func;
	NotifyHandle(EventSource<Evt> *parent, int id, Func callback) {
		this->m_parent = parent;
		this->m_callback = callback;
		this->m_id = id;
	};

	~NotifyHandle() { this->m_parent->cb_unregister(this->m_id); };

	void notify(Evt event) { this->m_callback(event); }

	EventSource<Evt> *m_parent;
	Func m_callback;
	int m_id;
};

template <typename Evt> class BaseEventSource : public EventSource<Evt> {
	typedef std::pair<int, Func> cb_container;

  public:
	BaseEventSource() {
		m_cbs = {};
		m_id_max = 1;
	};

	NotifyHandle<Evt> cb_register(Func callback) {
		NotifyHandle<Evt> handle(this, m_id_max, callback);
		m_cbs.push_back(cb_container(m_id_max, callback));
		m_id_max++;
		return std::move(handle);
	}

	void cb_unregister(int dereg_id) {
		std::vector<cb_container>::const_iterator it =
		    std::find_if(m_cbs.begin(), m_cbs.end(),
		                 [](cb_container c) { return c.first == dereg_id; });
		if (it != m_cbs.end())
			m_cbs.erase(it);
	}

protected:
    void Notify(Evt event) {
        for (auto it = m_cbs.begin(); it != m_cbs.end(); ++it) {
            it.right(event);
        }
    }

  private:
	int m_id_max;
	std::vector<cb_container> m_cbs;
};

#endif // UITEST_EVENTSOURCE_H
