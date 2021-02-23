#ifndef REFERENCEBIT_H
#define REFERENCEBIT_H

class ReferenceBit
{
	int bit;
public:
	ReferenceBit() : bit(-1)
	{
	}
	ReferenceBit(const ReferenceBit& rb) : bit(rb.bit)
	{
	}
public:
	bool isValid() const
	{
		return (bit != -1);
	}
	bool isInvalid() const
	{
		return (bit == -1);
	}
public:
	operator int() const
	{
		return bit;
	}
	ReferenceBit& operator = (int b)
	{
		bit = b;
		return *this;
	}
};

#endif // REFERENCEBIT_H
