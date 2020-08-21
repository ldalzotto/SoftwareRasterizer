
extern "C"
{
#include "v2/_interface/VectorStructuresC.h"
#include "v2/_interface/TransformC.h"
}

int main()
{
	Transform l_t1 = {}; Transform_Alloc(&l_t1);
	Transform l_t2 = {}; Transform_Alloc(&l_t2);
	Transform l_t3 = {}; Transform_Alloc(&l_t3);
	{
		Transform_AddChild(&l_t1, &l_t2);
		Transform_AddChild(&l_t3, &l_t2);
	}
	Transform_Free(&l_t1); Transform_Free(&l_t2); Transform_Free(&l_t3);
	
#if 0
	Array_Vector3f l_vec;
	Arr_Alloc_Vector3F(&l_vec, 10);
	Vector3f l_vecO = { 2.0f, 3.0f, 1.0f };
	Arr_PushBackRealloc_Vector3F(&l_vec, &l_vecO);
	l_vecO.y = 10.0f;
	Arr_PushBackRealloc_Vector3F(&l_vec, &l_vecO);

	Array_Vector3f_Iterator l_it; Arr_BuildIterator_Vector3F(&l_vec, &l_it);
	while (Iter_MoveNext_Vector3F(&l_it))
	{
		l_it.Current->y;
	}

	Arr_Free_Vector3F(&l_vec);
#endif
	return 0;
}