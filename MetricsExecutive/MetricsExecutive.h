#pragma once
#include <string>
#include <vector>
#include "../DataStore/DataStore.h"
#include "../FileMgr/FileMgr.h"
#include "../Parser/ConfigureParser.h"
#include "../MetricsAnalyzer/MetricsAnalyzer.h"

namespace Scanner
{
	class MetricsExecutive
	{
		MetricsAnalyzer MA;
	public:
		void PerformGranularAnalysis(int argc, char* argv[]);
		void FindAndAnalyzeFiles(std::string Path, std::string Pattern);
		void PerformFullAnalysis(int argc, char* argv[]);
		void FindAndAnalyzeFilesFull(std::string Path, std::string Pattern);
		void Displayfiles(int argc, char* argv[]);
		MetricsAnalyzer* GetAnalyzer()
		{
			return &MA;
		}
	};
}