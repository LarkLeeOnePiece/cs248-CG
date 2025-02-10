#include"mThreads.h"
//function for mthread
void matfprint(int row, int col, std::vector<int> data) {
	{
		for (int i = 0; i < row; i++) {
			for (int j = 0; j < col; ++j) {
				std::cout << data[i * row + j] << " ";
			}
			std::cout << std::endl;
		}
	}
}
void processPixel(int& pixel) {
	pixel = pixel * 2; // 以2倍的速度处理像素
}
void processPixels(ThreadSafeQueue<int>& queue, std::vector<int>& image) {
	while (true) {
		int pixel = queue.Pop();
		if (pixel == -1) {
			// 队列为空，线程退出
			break;
		}
		processPixel(pixel);
		// 更新图像中的像素
		// 实际图像处理操作将在此执行
		image.push_back(pixel);
	}
}
