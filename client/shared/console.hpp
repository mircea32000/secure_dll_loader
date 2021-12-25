#pragma once


namespace console {
	inline void initialize(const char* title)
	{
		AllocConsole();

		freopen_s(reinterpret_cast<_iobuf**>(__acrt_iob_func(0)), "conin$", "r", static_cast<_iobuf*>(__acrt_iob_func(0)));
		freopen_s(reinterpret_cast<_iobuf**>(__acrt_iob_func(1)), "conout$", "w", static_cast<_iobuf*>(__acrt_iob_func(1)));
		freopen_s(reinterpret_cast<_iobuf**>(__acrt_iob_func(2)), "conout$", "w", static_cast<_iobuf*>(__acrt_iob_func(2)));

		SetConsoleTitleA(title);
	}

	inline void release()
	{
		fclose(static_cast<_iobuf*>(__acrt_iob_func(0)));
		fclose(static_cast<_iobuf*>(__acrt_iob_func(1)));
		fclose(static_cast<_iobuf*>(__acrt_iob_func(2)));

		FreeConsole();
	}

	template <typename ... Args>
	void log(char const* const format, Args const& ... args) {
		printf(format, args ...);
	}

}