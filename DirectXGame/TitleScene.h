#pragma once
class TitleScene 
{
	public:
	void Initialize();

	void Update();

	void Draw();

	bool finished_ = false;
	bool ISFinished() const { return finished_;}

private:
	
};
