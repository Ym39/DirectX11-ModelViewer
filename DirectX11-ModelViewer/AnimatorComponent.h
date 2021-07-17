#pragma once
#include"Component.h"
#include"Utility.h"

class AnimatorComponent : public BaseComponent
{
public:
	DEFINE_COMPONENT(AnimatorComponent, BaseComponent, true);

public:
	AnimatorComponent():
		mAnimationData(nullptr),
		mIsPlay(false)
	{}

	virtual void Start() override {};
	virtual void Update(float deltaTime) override {};
	virtual void LateUpdate(float deltaTime) override
	{
		if (mIsPlay == false)
			return;

		static float lateTime = 0.0f;
		lateTime += deltaTime;

		if (lateTime >= 1000.0f / 30.0f)
		{
			mCurrentKeyframe++;
			if (mAnimationData->animationLength <= mCurrentKeyframe)
				mCurrentKeyframe = 0;

			lateTime = 0.0f;
		}
	}
	virtual void Destroy() override {};

	void SetAnimation(AnimationData* animation)
	{
		mAnimationData = animation;
		mCurrentKeyframe = 0;
	}

	const KeyFrameData& GetCurrentKeyAnimation(int index) const
	{
		return mAnimationData->keyFrames[index][mCurrentKeyframe];
	}

	void Play() { mIsPlay = true; }
	void Stop() { mIsPlay = false; }
protected:
	unsigned int mCurrentKeyframe;

private:
	AnimationData* mAnimationData;
	bool mIsPlay;
};

DECLARE_COMPONENT(AnimatorComponent);
