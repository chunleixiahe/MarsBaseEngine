#pragma once
#pragma once

/// @file
/// @brief Class mbe::UtilityAIComponent

#include <memory>
#include <map>
#include <limits>

#include <MBE/Core/Component.h>
#include <MBE/AI/AITask.h>

namespace mbe
{
	namespace detail
	{
		typedef std::size_t AITaskTypeID;

		constexpr AITaskTypeID UnspecifiedAITaskTypeID = std::numeric_limits<AITaskTypeID>::max();

		inline AITaskTypeID GetAITaskID() noexcept
		{
			// This will only be initialised once
			static AITaskTypeID lastId = 0;

			// After the first initialisation a new number will be returned for every function call
			return lastId++;
		}

		template <typename T>
		inline AITaskTypeID GetAITaskTypeID() noexcept
		{
			// There will be only one static variable for each template type
			static AITaskTypeID typeId = GetAITaskID();
			return typeId;
		}

	} //namespace detail


	class UtilityAIComponent : public Component
	{
	public:
		typedef std::shared_ptr<UtilityAIComponent> Ptr;
		typedef std::weak_ptr<UtilityAIComponent> WPtr;
		typedef std::unique_ptr<UtilityAIComponent> UPtr;

		typedef typename detail::AITaskTypeID AITaskTypeID;

	public:
		UtilityAIComponent(EventManager& eventManager, Entity& parentEntity);
		~UtilityAIComponent() = default;

	public:
		// Sets the current task aborting (if existing)
		// The task is added as the queued task
		// Creates a new task
		template<class TTask, typename... TArguments>
		void AddTask(TArguments&& ...arguments);

		// Sets the current task aborting (if existing)
		// The task is added as the queued task
		// Adds an existing task
		template<class TTask>
		void AddTask(typename TTask::Ptr taskPtr);

		// Check whether there is a next task using IsTaskQueued
		// Throws if no task is queued
		// Moves the next task 
		void MakeNextTaskActive();

		//template<class TAction, typename... TArguments>
		//void ReplaceAction(TArguments&& ...arguments);

		// Returns true if a task of type TTask is active
		template<class TTask>
		bool IsTaskActive() const;

		// Returns true if a task of type TTask is queued
		template<class TTask>
		bool IsTaskQueued() const;

		// Returns true if any task is active
		bool IsTaskActive() const;

		// Returns true if any task is queued
		bool IsTaskQueued() const;

		// Use IsTaskActive before
		// Throws if the task is not active
		template<class TTask>
		TTask& GetActiveTask();

		template<class TTask>
		const TTask& GetActiveTask() const;

		// Throws if no task is active
		AITask& GetActiveTask();
		const AITask& GetActiveTask() const;

		// Nullptr is returned if there is active task
		template<class TTask>
		typename TTask::Ptr GetActiveTaskPtr();

		// Nullptr is returned if there is active task
		AITask::Ptr GetActiveTaskPtr();

		// Throws if no task is active
		AITaskTypeID GetActiveTaskTypeID() const;

		// Use IsTaskQueued before
		// Throws if the task is not queued
		template<class TTask>
		TTask& GetQueuedTask();

		// const overload
		template<class TTask>
		const TTask& GetQueuedTask() const;

		// Throws if no task is queued
		AITask& GetQueuedTask();
		const AITask& GetQueuedTask() const;

		// Nullptr is returned if there is queued task
		template<class TTask>
		typename TTask::Ptr GetQueuedTaskPtr();

		// Nullptr is returned if there is queued task
		AITask::Ptr GetQueuedTaskPtr();

		// Throws if no task is active
		AITaskTypeID GetQueuedTaskTypeID() const;

		// Sets the task pointer to nullptr and the task type id to unspecified
		void ResetActiveTask();

		// Sets the task pointer to nullptr and the task type id to unspecified
		void ResetQueuedTask();

		template<class TTask>
		static AITaskTypeID GetAITaskTypeID();

	private:
		// If no task is assigned, the task pointer = nullptr and task type id = max value
		std::pair<AITask::Ptr, detail::AITaskTypeID> currentTask;
		std::pair<AITask::Ptr, detail::AITaskTypeID> nextTask;
	};


#pragma region Utility AI Component Template Implementations

	template<class TTask, typename ...TArguments>
	inline void UtilityAIComponent::AddTask(TArguments&& ...arguments)
	{
		auto taskPtr = std::make_shared<TTask>(std::forward<TArguments>(arguments)...);
		// Just for now
		nextTask.first = taskPtr;
		nextTask.second = detail::GetAITaskTypeID<TTask>();
		
		// Set the current task aborting
		if (IsTaskActive())
			currentTask.first->SetAborting();
	}

	template<class TTask>
	inline void UtilityAIComponent::AddTask(typename TTask::Ptr taskPtr)
	{
		nextTask.first = taskPtr;
		nextTask.second = detail::GetAITaskTypeID<TTask>();

		// Set the current task aborting
		if (IsTaskActive())
			currentTask.first->SetAborting();
	}

	template<class TTask>
	inline bool UtilityAIComponent::IsTaskActive() const
	{
		// Return true if a task of that type is active
		auto taskTypeId = detail::GetAITaskTypeID<TTask>();
		return currentTask.second == taskTypeId;
	}

	template<class TTask>
	inline bool UtilityAIComponent::IsTaskQueued() const
	{
		// Return true if a task of that type is queued
		auto taskTypeId = detail::GetAITaskTypeID<TTask>();
		return nextTask.second == taskTypeId;
	}


	template<class TTask>
	inline TTask& UtilityAIComponent::GetActiveTask()
	{
		// Throw if the task doesn't exist
		if (IsTaskActive<TTask>() == false)
			throw std::runtime_error("UtilityAIComponent: Currently there is no task active");

		assert(currentTask.first != nullptr && "UtilityAIComponent: Currently there is no task active");

		return *std::dynamic_pointer_cast<TTask>(currentTask.first);
	}

	template<class TTask>
	inline const TTask& UtilityAIComponent::GetActiveTask() const
	{
		// Throw if the task doesn't exist
		if (IsTaskActive<TTask>() == false)
			throw std::runtime_error("UtilityAIComponent: Currently there is no task active");

		assert(currentTask.first != nullptr && "UtilityAIComponent: Currently there is no task active");

		return *std::dynamic_pointer_cast<TTask::Ptr>(currentTask.first);
	}

	template<class TTask>
	inline typename TTask::Ptr UtilityAIComponent::GetActiveTaskPtr()
	{
		return std::dynamic_pointer_cast<TTask::Ptr>(currentTask.first);
	}

	template<class TTask>
	inline TTask& UtilityAIComponent::GetQueuedTask()
	{
		// Throw if the task doesn't exist
		if (IsTaskQueued<TTask>() == false)
			throw std::runtime_error("UtilityAIComponent: Currently there is no task queued");

		assert(nextTask.first != nullptr && "UtilityAIComponent: Currently there is no task queued");

		return *std::dynamic_pointer_cast<TTask::Ptr>(nextTask.first);
	}

	template<class TTask>
	inline const TTask& UtilityAIComponent::GetQueuedTask() const
	{
		// Throw if the task doesn't exist
		if (IsTaskQueued<TTask>() == false)
			throw std::runtime_error("UtilityAIComponent: Currently there is no task queued");

		assert(nextTask.first != nullptr && "UtilityAIComponent: Currently there is no task queued");

		return *std::dynamic_pointer_cast<TTask::Ptr>(nextTask.first);
	}

	template<class TTask>
	inline typename TTask::Ptr UtilityAIComponent::GetQueuedTaskPtr()
	{
		return std::dynamic_pointer_cast<TTask::Ptr>(nextTask.first);
	}

	template<class TTask>
	inline typename UtilityAIComponent::AITaskTypeID UtilityAIComponent::GetAITaskTypeID()
	{
		return detail::GetAITaskTypeID<TTask>();
	}

#pragma endregion


} //namespace mbe