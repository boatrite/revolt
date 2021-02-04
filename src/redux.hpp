/*
Copyright (c) 2016 Samuel Utbult
Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef REDUX_H
#define REDUX_H

#include <functional>
#include <vector>

namespace redux {

template <typename STATE_T, typename ACTION_T>
class Store {
	public:
		typedef std::function<void(STATE_T&)> Subscriber;

		Store();
		void subscribe(Subscriber);
		void dispatch(ACTION_T);
		STATE_T& getState();

	private:
		STATE_T state {};
		std::vector<Subscriber> subscribers;
};

template <typename STATE_T, typename ACTION_T>
Store<STATE_T, ACTION_T>::Store() {}

template <typename STATE_T, typename ACTION_T>
void Store<STATE_T, ACTION_T>::subscribe(Subscriber subscriber) {
	subscribers.push_back(subscriber);
}

template <typename STATE_T, typename ACTION_T>
void Store<STATE_T, ACTION_T>::dispatch(ACTION_T action) {
	action(state);

	for(int i = 0; i < subscribers.size(); i++) {
		subscribers[i](state);
	}
}

template <typename STATE_T, typename ACTION_T>
STATE_T& Store<STATE_T, ACTION_T>::getState() {
	return state;
}

};

#endif
