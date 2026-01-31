#pragma once

#include <mutex>
#include <queue>
#include <optional>

#include "glm/mat4x4.hpp"
#include "helpers/ID.hpp"
#include "gpuInterface/Model.hpp"
#include "helpers/math/Vector3.hpp"
#include "helpers/math/Quaternion.hpp"


class RenderingCache
{
public:
	RenderingCache() = default;

	struct FrameData {
		struct ComponentData {
			ID::UUID id;
			Vector3 position;
			Quaternion orientation;
			Vector3 scale;

			Vector3 velocity;
			Vector3 angularMomentum;

			// when adding a new model to the DataArray<Model3d> the pointer may become invalid if the array resizes
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

	size_t size() {
		std::lock_guard<std::mutex> lock(mutex_);
		return _frames.size();
	}
	
	void invalidate() {
		std::lock_guard<std::mutex> lock(mutex_);
		while (!_frames.empty()) {
			_frames.pop();
		}
	}

private:
	std::queue<FrameData> _frames;
	std::mutex mutex_;
	std::atomic<unsigned int> _maxFrames = 100;
};