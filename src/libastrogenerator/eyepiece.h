#ifndef EYEPIECE_H
#define EYEPIECE_H

class Eyepiece
{
public:
	explicit constexpr Eyepiece(int focalLength) :
	    m_focalLength(focalLength)
	{
	}

	[[nodiscard]] constexpr int focalLength() const noexcept
	{
		return m_focalLength;
	}

private:
	int m_focalLength;
};

#endif // EYEPIECE_H
