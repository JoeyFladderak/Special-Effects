#ifndef MD2READER
#define MD2READER

#include <stdio.h>
#include <string>
#include "AnimatedModel.h"

#ifndef byte
typedef unsigned char byte;
#endif /* byte */

struct md2_model_t;		// forward declaration, definition is in cpp file

class md2Reader
{
public:
	md2Reader();
	~md2Reader();

	int Open(const char filename[]);
	void Read( AnimatedModel* md2mesh );

private:
	void md2_freeModel(md2_model_t *model);
	void BuildAnimations( md2_model_t *model, AnimatedModel* md2mesh );
	AnimatedModel::AnimationType GetAnimationTypeFromFullName( const char* name );

	int m_totsize;
	FILE* m_File;
};



#endif
