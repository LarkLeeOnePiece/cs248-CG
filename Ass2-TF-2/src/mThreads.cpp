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
	pixel = pixel * 2; // ��2�����ٶȴ�������
}
void processPixels(ThreadSafeQueue<int>& queue, std::vector<int>& image) {
	while (true) {
		int pixel = queue.Pop();
		if (pixel == -1) {
			// ����Ϊ�գ��߳��˳�
			break;
		}
		processPixel(pixel);
		// ����ͼ���е�����
		// ʵ��ͼ����������ڴ�ִ��
		image.push_back(pixel);
	}
}
