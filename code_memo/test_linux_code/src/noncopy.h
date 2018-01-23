#ifndef _NON_COPY_H_
#define _NON_COPY_H_

class CNoncopy
{
public:
	CNoncopy() {}
	virtual ~CNoncopy() {}

protected:
	CNoncopy& operator = (CNoncopy& rhs);
	CNoncopy(CNoncopy& rhs);
};

#endif