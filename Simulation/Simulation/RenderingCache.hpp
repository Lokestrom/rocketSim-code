#pragma once

#include <mutex>
#include <queue>
#include <optional>

#include "glm/mat4x4.hpp"
#include "helpers/ID.hpp"
#include "gpuInterface/Model.hpp"


class RenderingCache
{
public:
	RenderingCache() = default;

	struct FrameData {
		struct ComponentData {
			ID::UUID id;
			glm::mat4 transform;
			// never changed so safe
			// but in the future create a model cache on the gpu side
			GPU::Model3D* model;
		};
		double time;
		std::vector<ComponentData> components;
	};
	void addFrameData(const FrameData& data) {
		std::lock_guard<std::mutex> lock(mutex_);
		_frames.push(data);
		assert(_frames.size() <= _maxFrames && "RenderingCache::addFrameData: Frame cache overflow");
	}
	std::optional<FrameData> getFrameData(double time) {
		std::lock_guard<std::mutex> lock(mutex_);
		if (_frames.empty()) {
			return std::optional<FrameData>();
		}
		FrameData frame = _frames.front();
		if (frame.time > time) {
			return std::optional<FrameData>();
		}
		_frames.pop();
		while (frame.time < time && !_frames.empty()) {
			frame = _frames.front();
			_frames.pop();
		} 

		return frame;
	}

	bool isEmpty() {
		return _frames.empty();
	}
	bool isFull() {
		return _frames.size() == _maxFrames;
	}

private:
	std::queue<FrameData> _frames;
	std::mutex mutex_;
	std::atomic<unsigned int> _maxFrames = 100;
};