#include "fileSystem.hpp"

namespace fileSystem {
#if debug
	void writeError(error e, exitCodes code) {
		std::cout << e.what << "\n";
		if (e.level >= loadingVar::exitLevel)
			std::exit(code);
	}
#else
	void writeError(error e, exitCodes code) {
		objects::errorLogFile << e.what << "\n";
		if (e.level >= loadingVar::exitLevel)
			std::exit(code);
	}
#endif

	Vector<String> returnVariableAndValue(String line) {
		line.remove(' ');
		Vector<String> ans = line.split('=');
		if (ans.size() > 2)
			throw error("Too many arguments. Not alowed", mid);
		ans[0].lower();
		ans[1].lower();
		return ans;
	}

	Vector<String> returnArgs(String args) {
		args = args.remove(' ');
		args.pop(0, args.linearSearch('('));
		args.pop(args.linearSearchR(')'), args.length() - 1);
		Vector<String> argv = args.split(',');
		for (auto i = 0; i < argv.size(); i++) {
			if (argv[i].contains("{")) {
				String newArg = "";
				auto j = i;
				for (; !argv[j].contains("}"); j++) {
					newArg += argv[j] + ",";
				}
				newArg += argv[j];
				argv.pop(i, j + 1);
				argv.insert(i, newArg);
			}
		}
		return argv;
	}

	Vector<int> returnVectori(String arg) {
		arg.popBack();
		arg = arg.remove(' ');
		arg.pop(0);
		Vector<String> vec = std::move(arg.split(','));
		Vector<int> ans(vec.size());
		for (const auto& i : vec) {
			ans.pushBack(SToi(i));
		}
		return ans;
	}

	Vector3 returnVector3(String arg) {
		if (arg.split('{')[0].length() != arg.length() - 1) {
			throw error("The arg is labeld with : " + arg.split('{')[0] + "may not be a vector3", low);
		}
		arg.popBack();
		arg.pop(0);
		arg = arg.remove(' ');
		return { STold(arg.split(',')[0]), STold(arg.split(',')[1]), STold(arg.split(',')[2]) };
	}

	Quaternion returnQuaternion(String arg) {
		if (arg.split('{')[0] != "quat") {
			throw error("The arg is not labeld a Quaternion. The arg may not be a quaternion", low);
		}
		arg.popBack();
		arg.pop(0);
		arg = arg.remove(' ');
		return toQuaternion({ degToRad(STold(arg.split(',')[0])), degToRad(STold(arg.split(',')[1])), degToRad(STold(arg.split(',')[2])) });
	}
} //fileSystem