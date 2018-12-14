#include <iostream>
#include <vector>

#include "vtkSmartPointer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkNIFTIImageReader.h"
#include "vtkGPUVolumeRayCastMapper.h"
#include "vtkPiecewiseFunction.h"
#include "vtkVolumeProperty.h"
#include "vtkVolume.h"
#include "vtkColorTransferFunction.h"
#include "vtkImageConnectivityFilter.h"
#include "vtkImageData.h"
#include "vtkImageMathematics.h"
#include "vtkImageIterator.h"
#include "vtkImageCast.h"
#include "vtkImageLogic.h"
#include "vtkMarchingCubes.h"
#include "vtkProperty.h"
#include "vtkPolyDataMapper.h"
#include "vtkSmoothPolyDataFilter.h"
#include "vtkPolyDataNormals.h"

using namespace std;

int main()
{
	// 设置肝脏八段的颜色
	double colors[8][3] = {
		{1, 0, 0},
		{0, 1, 0},
		{0, 0, 1},
		{1, 1, 0},
		{0, 1, 1},
		{1, 0, 1},
		{0.5, 1, 0.5},
		{1, 1, 0.5} };

	string fileName = "../data/liversegmentresult.nii.gz";

	// 读取原始数据
	vtkSmartPointer<vtkNIFTIImageReader> niiReader = vtkSmartPointer<vtkNIFTIImageReader>::New();
	niiReader->SetFileName(fileName.c_str());
	niiReader->Update();
	
	// 体绘制部分
	vtkSmartPointer<vtkImageConnectivityFilter> connectivityFilter = vtkSmartPointer<vtkImageConnectivityFilter>::New();
	connectivityFilter->SetScalarRange(1, 8);
	connectivityFilter->SetLabelModeToConstantValue();
	connectivityFilter->SetLabelConstantValue(1);
	connectivityFilter->SetExtractionModeToLargestRegion();
	connectivityFilter->SetInputConnection(niiReader->GetOutputPort());
	connectivityFilter->Update();

	vtkSmartPointer<vtkImageCast> imgCast = vtkSmartPointer<vtkImageCast>::New();
	imgCast->SetInputConnection(connectivityFilter->GetOutputPort());
	imgCast->SetOutputScalarTypeToShort();
	imgCast->Update();

	vtkSmartPointer<vtkImageMathematics> math = vtkSmartPointer<vtkImageMathematics>::New();
	math->SetInputData(0, niiReader->GetOutput());
	math->SetInputData(1, imgCast->GetOutput());
	math->SetOperationToMultiply();
	math->Update();

	vtkSmartPointer<vtkGPUVolumeRayCastMapper> Mapper = vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
	Mapper->SetInputConnection(math->GetOutputPort());
		
	vtkSmartPointer<vtkPiecewiseFunction> opacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
	opacity->AddPoint(0, 0);
	opacity->AddPoint(1, 0.5);
	opacity->AddPoint(2, 0.5);
	opacity->AddPoint(3, 0.5);
	opacity->AddPoint(4, 0.5);
	opacity->AddPoint(5, 0.5);
	opacity->AddPoint(6, 0.5);
	opacity->AddPoint(7, 0.5);
	opacity->AddPoint(8, 0.5);

	vtkSmartPointer<vtkColorTransferFunction> color = vtkSmartPointer<vtkColorTransferFunction>::New();
	for (int i = 0; i < 8; i++) color->AddRGBPoint(i, colors[i][0], colors[i][1], colors[i][2]);
	
	vtkSmartPointer<vtkVolumeProperty> property = vtkSmartPointer<vtkVolumeProperty>::New();
	property->SetScalarOpacity(opacity);
	property->SetColor(color);
	property->ShadeOn();
	property->SetDiffuse(0.6);
	property->SetSpecular(0.5);
	property->SetAmbient(0.5);
	
	vtkSmartPointer<vtkVolume> volume = vtkSmartPointer<vtkVolume>::New();
	volume->SetMapper(Mapper);
	volume->SetProperty(property);

	// 面绘制部分
	vector<vtkSmartPointer<vtkActor>> Actors;
	for (int surfaceIndex = 1; surfaceIndex < 8; surfaceIndex++)
	{
		vtkSmartPointer<vtkImageConnectivityFilter> surfaceFilter = vtkSmartPointer<vtkImageConnectivityFilter>::New();
		surfaceFilter->SetScalarRange(surfaceIndex, surfaceIndex);
		surfaceFilter->SetExtractionModeToLargestRegion();
		surfaceFilter->SetInputConnection(math->GetOutputPort());
		surfaceFilter->SetLabelModeToConstantValue();
		surfaceFilter->SetLabelConstantValue(1);
		surfaceFilter->Update();

		vtkSmartPointer<vtkMarchingCubes> mc = vtkSmartPointer<vtkMarchingCubes>::New();
		mc->SetValue(0, 1);
		mc->SetInputConnection(surfaceFilter->GetOutputPort());
		mc->Update();

		vtkSmartPointer<vtkSmoothPolyDataFilter> smoothFilter = vtkSmartPointer<vtkSmoothPolyDataFilter>::New();
		smoothFilter->SetInputConnection(mc->GetOutputPort());
		smoothFilter->SetNumberOfIterations(15);
		smoothFilter->SetRelaxationFactor(0.1);
		smoothFilter->FeatureEdgeSmoothingOff();
		smoothFilter->BoundarySmoothingOn();
		smoothFilter->Update();
		
		vtkSmartPointer<vtkPolyDataNormals> normalGenerator = vtkSmartPointer<vtkPolyDataNormals>::New();
		normalGenerator->SetInputConnection(smoothFilter->GetOutputPort());
		normalGenerator->ComputePointNormalsOn();
		normalGenerator->ComputeCellNormalsOn();
		normalGenerator->Update();

		vtkSmartPointer<vtkPolyDataMapper> Mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		Mapper->SetInputConnection(normalGenerator->GetOutputPort());
		Mapper->ScalarVisibilityOff();

		vtkSmartPointer<vtkProperty> Prop = vtkSmartPointer<vtkProperty>::New();
		Prop->SetColor(colors[surfaceIndex]);

		vtkSmartPointer<vtkActor> Actor = vtkSmartPointer<vtkActor>::New();
		Actor->SetMapper(Mapper);
		Actor->SetProperty(Prop);

		Actors.push_back(Actor);
	}

	// 体绘制renderer
	vtkSmartPointer<vtkRenderer> renderer1 = vtkSmartPointer<vtkRenderer>::New();
	renderer1->AddActor(volume);
	renderer1->SetBackground(0.1, 0.2, 0.4);
	renderer1->SetViewport(0, 0, 0.49, 1);

	// 面绘制renderer
	vtkSmartPointer<vtkRenderer> renderer2 = vtkSmartPointer<vtkRenderer>::New();
	for (int i = 0; i < Actors.size(); i++)renderer2->AddActor(Actors[i]);
	renderer2->SetActiveCamera(renderer1->GetActiveCamera());
	renderer2->ResetCamera();
	renderer2->SetBackground(0.1, 0.2, 0.4);
	renderer2->SetViewport(0.51, 0, 1, 1);

	vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
	renWin->AddRenderer(renderer1);
	renWin->AddRenderer(renderer2);
	renWin->SetSize(600, 300);

	vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	iren->SetRenderWindow(renWin);

	vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	iren->SetInteractorStyle(style);
	iren->Initialize();
	iren->Start();

	return EXIT_SUCCESS;
}