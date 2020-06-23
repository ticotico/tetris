#ifndef __KEYOPERATION_H__
#define __KEYOPERATION_H__

#include <windows.h>
#include <list>
#include <functional>
#include <memory>

//A calss stores a callback function when KeyState is notified.
class KeyObserver
{
public:
	//constructure
	//param fnCallback: the callbeck function.
	explicit KeyObserver(std::function<void(void)> fnCallback);

	//Call the stored callback function.
	void Update();
private:
	std::function<void(void)> m_fnCallback;
};

//when key is not press, m_eState is set to State::Free.
//when key is pressed and m_eState is equal State::Free, notify and 
class KeyState
{
public:
	//constructure
	//param iKey: windows Key definition such as VK_SPACE.
	explicit KeyState(int key);

	//Add an observer which Update() method is called when KeyState::NotifyAllOberserver().
	//param wpObserver: weak pointer will be added.
	void AddObserver(std::weak_ptr<KeyObserver> wpObserver);

	//Delete an observer.
	//param wpObserver: weak pointer will be deleted.
	void DeleteObserver(std::weak_ptr<KeyObserver> wpObserver);

	//Check key is pressed and whether notify observers.
	void Check();

	//Check key is pressed or not.
	bool IsKeyPressed() const;

	//All KeyObserver::Update() will be called.
	void NotifyAllOberserver();
private:
	//For obtain most significant bit of GetAsyncKeyState return value.
	static constexpr SHORT eMask = static_cast<SHORT>(1 << (sizeof(SHORT) * 8 - 1));

	//Notify Behavior while press sustained
	//From Free state, Notify() directly and change state to OnPress.
	//From OnPress state,  Notify() after eOnPressNotifyCount(3) periods and chage state to DeepPress
	//At DeepPress state, Notify() once per eDeepPressNotifyCount(2) periods.
	static constexpr std::size_t eOnPressNotifyCount = 3;
	static constexpr std::size_t eDeepPressNotifyCount = 2;
	enum class State : char
	{
		Free,
		OnPress,
		DeepPress
	};
	int m_iKey;
	State m_eState;
	std::size_t m_szPressCount;
	std::list<std::weak_ptr<KeyObserver>> m_lstObserver;
};
#endif
