#pragma once

#include <GameCore/GenericHandlesDynamicArray.h>

namespace TempTests
{
	struct VertexHandleTag {};
	typedef SDK::GenericHandle<int, 12, 20, VertexHandleTag> VertexBufferHandle;

	typedef SDK::GenericHandleDynamicArray<VertexBufferHandle, int> VertexBuffers;
	typedef SDK::GenericHandleDynamicArray<VertexBufferHandle, std::unique_ptr<int>> VertexBuffersPtrs;

	void Test()
	{
		VertexBuffers bufs;
		auto handle_i = bufs.CreateNew(5);
		int* ptr_i = bufs.Access(handle_i);
		bufs.Destroy(handle_i);

		VertexBuffersPtrs ptr_bufs;
		auto handle_ptr = ptr_bufs.CreateNew(9);
		int* ptr_i_ptr = ptr_bufs.Access(handle_ptr);
		ptr_bufs.Destroy(handle_ptr);
	}


} // TempTests
