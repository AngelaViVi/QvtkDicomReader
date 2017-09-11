#pragma once
#include "vtkInteractorStyleImage.h"
#include <vtkTextMapper.h>
#include <vtkImageViewer2.h>
#include <sstream>
#include <vtkObjectFactory.h>
#include <vtkSmartPointer.h>

#include <vtkSmartPointer.h>
#include <vtkObjectFactory.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkActor.h>
// headers needed for this example
#include <vtkImageViewer2.h>
#include <vtkDICOMImageReader.h>
#include <vtkInteractorStyleImage.h>
#include <vtkActor2D.h>
#include <vtkTextProperty.h>
#include <vtkTextMapper.h>
// needed to easily convert int to std::string
#include <sstream>
class StatusMessage {
public:
	static std::string Format(int slice, int maxSlice) {
		std::stringstream tmp;
		tmp << "Slice Number  " << slice + 1 << "/" << maxSlice + 1;
		return tmp.str();
	}
};

class myVtkInteractorStyleImage :public vtkInteractorStyleImage
{
public:
	static myVtkInteractorStyleImage* New();
	vtkTypeMacro(myVtkInteractorStyleImage, vtkInteractorStyleImage);
	enum MOUSEFUNC
	{
		POINTER,
		ZOOM,
		GRAYLEVEL,
		MOVE
	}MouseFunction;
protected:
	vtkImageViewer2* _ImageViewer;
	vtkTextMapper* _StatusMapper;
	int _Slice;
	int _MinSlice;
	int _MaxSlice;

public:
	void SetImageViewer(vtkImageViewer2* imageViewer);
	void SetStatusMapper(vtkTextMapper* statusMapper);
	void MoveSliceForward();
	void MoveSliceBackward();
	void AddUpdate(void(*pfun)(void));//
	
protected:
	void OnUpdate();//ÿ����Ҫ���µ�ʱ��,�����������
	void(*funcinupdate)(void);
	virtual void OnKeyDown()override;
	virtual void OnMouseWheelForward()override;
	virtual void OnMouseWheelBackward()override;

	//��������İ��º�̧��
	virtual void OnLeftButtonDown() override;
	virtual void OnLeftButtonUp() override;
	virtual void OnMiddleButtonDown() override;
	virtual void OnMiddleButtonUp() override;
	virtual void OnRightButtonDown() override;
	virtual void OnRightButtonUp() override;

};

