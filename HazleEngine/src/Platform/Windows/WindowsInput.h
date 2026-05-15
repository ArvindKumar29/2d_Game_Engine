#pragma once

#include "Hazle/Input.h"

namespace Hazle
{
	class WindowsInput : public Hazle::Input
	{
	protected:
		virtual bool IsKeyPressedImpl(int keycode) override;
		virtual std::pair<float, float> GetMousePositionImpl() override;
		virtual bool IsMouseButtonPressedImpl(int button) override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;

	};
};

