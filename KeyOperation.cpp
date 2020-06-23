#include "KeyOperation.h"

//obtain whether two weak pointer has the same owner
template <typename T, typename U>
static inline bool equals(const std::weak_ptr<T>& t, const std::weak_ptr<U>& u)
{
	return !t.owner_before(u) && !u.owner_before(t);
}

//=========================================
KeyObserver::KeyObserver(std::function<void(void)> fnCallback)
	: m_fnCallback(fnCallback)
{
}

void KeyObserver::Update()
{
	if (m_fnCallback)
		m_fnCallback();
}

//=========================================
KeyState::KeyState(int key) : m_iKey(key), m_eState(State::Free), m_szPressCount(0)
{
}

void KeyState::AddObserver(std::weak_ptr<KeyObserver> wpObserver)
{
	auto iter = std::find_if(m_lstObserver.begin(), m_lstObserver.end(),
		[wpObserver](std::weak_ptr<KeyObserver> other) { return equals(wpObserver, other); });
	if (iter == m_lstObserver.end())
		m_lstObserver.push_back(wpObserver);
}

void KeyState::DeleteObserver(std::weak_ptr<KeyObserver> wpObserver)
{
	auto iter = std::find_if(m_lstObserver.begin(), m_lstObserver.end(),
		[wpObserver](std::weak_ptr<KeyObserver> other) { return equals(wpObserver, other); });
	if (iter != m_lstObserver.end())
		m_lstObserver.erase(iter);
}

void KeyState::Check()
{
	if (!IsKeyPressed())
	{
		m_eState = State::Free;
		m_szPressCount = 0;
	}
	else
	{
		switch (m_eState)
		{
		case State::Free:
			m_eState = State::OnPress;
			++m_szPressCount;
			NotifyAllOberserver();
			break;
		case State::OnPress:
			if (m_szPressCount == eOnPressNotifyCount)
			{
				m_eState = State::DeepPress;
				m_szPressCount = 1;
				NotifyAllOberserver();
			}
			else
			{
				++m_szPressCount;
			}
			break;
		case State::DeepPress:
			if (m_szPressCount == eDeepPressNotifyCount)
			{
				//m_eState still be State::DeepPress
				m_szPressCount = 1;
				NotifyAllOberserver();
			}
			else
			{
				++m_szPressCount;
			}
			break;
		}
	}
}

bool KeyState::IsKeyPressed() const
{
	SHORT keyState = GetAsyncKeyState(m_iKey);
	return (keyState & eMask) != 0;
}

void KeyState::NotifyAllOberserver()
{
	auto iter = m_lstObserver.begin();
	for (; iter != m_lstObserver.end(); ++iter)
	{
		if (!iter->expired())
		{
			auto spObserver = iter->lock();
			spObserver->Update();
		}
	}
}
