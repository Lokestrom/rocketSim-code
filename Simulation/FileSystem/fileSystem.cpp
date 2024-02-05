#include "fileSystem.hpp"

namespace fileSystem {
	Vector<String> returnVariableAndValue(String line) {
		line.remove(' ');
		line.lower();
		Vector<String> ans = line.split('=');
		if (ans.size() > 2)
			Error("Too many arguments. Arguments: " + line, Error::exitCodes::badUserBehavior);
		return ans;
	}

	Vector<String> returnArgs(String args) {
		args.remove(' ');
		if(args.contains("("))
			args.pop(0, args.linearSearch('(')+1);
		args.pop(args.linearSearchR(')'), args.length());
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
		arg.remove(' ');
		arg.pop(0);
		Vector<String> vec = arg.split(',');
		Vector<int> ans(vec.size());
		for (const auto& i : vec) {
			ans.pushBack(SToi(i));
		}
		return ans;
	}
	Vector<ID::ID_T> returnVectorID(String arg) {
		arg.popBack();
		arg.remove(' ');
		arg.pop(0);
		Vector<String> vec = arg.split(',');
		Vector<ID::ID_T> ans(vec.size());
		for (const auto& i : vec) {
			ans.pushBack(SToi(i));
		}
		return ans;
	}

	Vector3 returnVector3(String arg) {
		if (arg.split('{').size() != 1) {
			Error("The arg is labeld with: " + arg.split('{')[0] + " may not be a vector3", Error::exitCodes::badUserBehavior, Error::recoveryType::ignore);
		}
		arg.popBack();
		arg.remove(' ');
		arg.pop(0);
		return { STold(arg.split(',')[0]), STold(arg.split(',')[1]), STold(arg.split(',')[2]) };
	}

	Quaternion returnQuaternion(String arg) {
		if (arg.split('{')[0] != "quat") {
			Error("The arg is not labeld a Quaternion. The arg may not be a quaternion", Error::exitCodes::badUserBehavior, Error::recoveryType::ignore);
		}
		arg.popBack();
		arg.remove(' ');
		arg.pop(0);
		return toQuaternion({ degToRad(STold(arg.split(',')[0])), degToRad(STold(arg.split(',')[1])), degToRad(STold(arg.split(',')[2])) });
	}
} //fileSystem