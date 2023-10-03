#pragma once
#ifdef _WIN32
	#define CROW_API __declspec(dllexport)
#else
	#define CROW_API
#endif