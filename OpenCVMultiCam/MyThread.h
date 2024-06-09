#pragma once
#include <thread>
#include <future>

struct MyThread
{
    CString className{};
	bool Run{ false };
	std::future<void> FutureObject;
	std::promise<void> ExitSignal;
	std::unique_ptr<std::thread> Thread{ nullptr };

    explicit MyThread(const CString& name) : className(name) {}
    void Destroy() {
        if (Thread != nullptr) {
            ExitSignal.set_value();
            Run = false;
            //if (Thread->joinable()) {
                Thread->join();
                TRACE(_T("$$$") + className + _T(" Destroy....\n"));
           // }
            Thread = nullptr;
        }
    }
};

