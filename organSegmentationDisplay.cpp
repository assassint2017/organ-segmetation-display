//#include "vtkSmartPointer.h"
//#include "vtkRenderWindow.h"
//#include "vtkRenderer.h"
//#include "vtkRenderWindowInteractor.h"
//#include "vtkInteractorStyleTrackballCamera.h"
//#include "vtkNIFTIImageReader.h"
//#include "vtkPolyDataMapper.h"
//#include "vtkActor.h"
//#include "vtkMarchingCubes.h"
//#include "vtkProperty.h"
//#include "vtkImageConnectivityFilter.h"
//#include "vtkImageData.h"
//#include "vtkFillHolesFilter.h"
//#include "vtkAppendPolyData.h"
//#include "vtkPolyDataConnectivityFilter.h"
//#include "vtkSmoothPolyDataFilter.h"
//#include "vtkPolyDataNormals.h"
//#include "vtkCamera.h"
//#include "vtkOBBTree.h"
//
//#include <iostream>
//#include <vector>
//
//using namespace std;
//
//int main()
//{
//	// 定义各部位的渲染颜色
//	double liverColor[] = {1, 0, 0};
//	double leftKidneyColor[] = { 1, 1, 0 };
//	double rightKidneyColor[] = { 0, 1, 1 };
//	double livertumorColor[] = { 0, 1, 0 };
//
//	// 读取金标准文件
//	string fileName = "C:/Users/14595/Desktop/多器官分割数据/1206T1VenousPhase-pred.nii.gz";
//	vtkSmartPointer<vtkNIFTIImageReader> niiReader = vtkSmartPointer<vtkNIFTIImageReader>::New();
//	niiReader->SetFileName(fileName.c_str());
//	niiReader->Update();
//
//	// 提取肝脏
//	vtkSmartPointer<vtkImageConnectivityFilter> liverFilter = vtkSmartPointer<vtkImageConnectivityFilter>::New();
//	liverFilter->SetScalarRange(1, 1);
//	liverFilter->SetExtractionModeToLargestRegion();
//	liverFilter->SetInputData(niiReader->GetOutput());
//	liverFilter->Update();
//
//	vtkSmartPointer<vtkMarchingCubes> liverMC = vtkSmartPointer<vtkMarchingCubes>::New();
//	liverMC->SetInputConnection(liverFilter->GetOutputPort());
//	liverMC->SetValue(0, 1);
//	liverMC->Update();
//
//	vtkSmartPointer<vtkSmoothPolyDataFilter> liversmoothFilter = vtkSmartPointer<vtkSmoothPolyDataFilter>::New();
//	liversmoothFilter->SetInputConnection(liverMC->GetOutputPort());
//	liversmoothFilter->SetNumberOfIterations(15);
//	liversmoothFilter->SetRelaxationFactor(0.1);
//	liversmoothFilter->FeatureEdgeSmoothingOff();
//	liversmoothFilter->BoundarySmoothingOn();
//	liversmoothFilter->Update();
//
//	vtkSmartPointer<vtkPolyDataNormals> livernormalGenerator = vtkSmartPointer<vtkPolyDataNormals>::New();
//	livernormalGenerator->SetInputConnection(liversmoothFilter->GetOutputPort());
//	livernormalGenerator->ComputePointNormalsOn();
//	livernormalGenerator->ComputeCellNormalsOn();
//	livernormalGenerator->Update();
//
//	vtkSmartPointer<vtkPolyDataMapper> liverMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
//	liverMapper->SetInputConnection(livernormalGenerator->GetOutputPort());
//	liverMapper->ScalarVisibilityOff();
//
//	vtkSmartPointer<vtkProperty> liverProp = vtkSmartPointer<vtkProperty>::New();
//	liverProp->SetColor(liverColor);
//	liverProp->SetOpacity(0.4);
//
//	vtkSmartPointer<vtkActor> liverActor = vtkSmartPointer<vtkActor>::New();
//	liverActor->SetMapper(liverMapper);
//	liverActor->SetProperty(liverProp);
//
//	// 提取左肾
//	vtkSmartPointer<vtkImageConnectivityFilter> leftKidneyFilter = vtkSmartPointer<vtkImageConnectivityFilter>::New();
//	leftKidneyFilter->SetScalarRange(2, 2);
//	leftKidneyFilter->SetExtractionModeToLargestRegion();
//	leftKidneyFilter->SetInputData(niiReader->GetOutput());
//	leftKidneyFilter->Update();
//
//	vtkSmartPointer<vtkMarchingCubes> leftKidneyMC = vtkSmartPointer<vtkMarchingCubes>::New();
//	leftKidneyMC->SetInputConnection(leftKidneyFilter->GetOutputPort());
//	leftKidneyMC->SetValue(0, 1);
//	leftKidneyMC->Update();
//
//	vtkSmartPointer<vtkFillHolesFilter> leftKidneyfillHoles = vtkSmartPointer<vtkFillHolesFilter>::New();
//	leftKidneyfillHoles->SetHoleSize(1e3);
//	leftKidneyfillHoles->SetInputData(leftKidneyMC->GetOutput());
//	leftKidneyfillHoles->Update();
//
//	vtkSmartPointer<vtkSmoothPolyDataFilter> leftKidneysmoothFilter = vtkSmartPointer<vtkSmoothPolyDataFilter>::New();
//	leftKidneysmoothFilter->SetInputConnection(leftKidneyfillHoles->GetOutputPort());
//	leftKidneysmoothFilter->SetNumberOfIterations(15);
//	leftKidneysmoothFilter->SetRelaxationFactor(0.1);
//	leftKidneysmoothFilter->FeatureEdgeSmoothingOff();
//	leftKidneysmoothFilter->BoundarySmoothingOn();
//	leftKidneysmoothFilter->Update();
//	
//	vtkSmartPointer<vtkPolyDataNormals> leftKidneynormalGenerator = vtkSmartPointer<vtkPolyDataNormals>::New();
//	leftKidneynormalGenerator->SetInputConnection(leftKidneysmoothFilter->GetOutputPort());
//	leftKidneynormalGenerator->ComputePointNormalsOn();
//	leftKidneynormalGenerator->ComputeCellNormalsOn();
//	leftKidneynormalGenerator->Update();
//
//	vtkSmartPointer<vtkPolyDataMapper> leftKidneyMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
//	leftKidneyMapper->SetInputConnection(leftKidneynormalGenerator->GetOutputPort());
//	leftKidneyMapper->ScalarVisibilityOff();
//
//	vtkSmartPointer<vtkProperty> leftKidneyProp = vtkSmartPointer<vtkProperty>::New();
//	leftKidneyProp->SetColor(leftKidneyColor);
//	leftKidneyProp->SetOpacity(0.5);
//
//	vtkSmartPointer<vtkActor> leftKidneyActor = vtkSmartPointer<vtkActor>::New();
//	leftKidneyActor->SetMapper(leftKidneyMapper);
//	leftKidneyActor->SetProperty(leftKidneyProp);
//
//	// 提取左肾
//	vtkSmartPointer<vtkImageConnectivityFilter> rightKidneyFilter = vtkSmartPointer<vtkImageConnectivityFilter>::New();
//	rightKidneyFilter->SetScalarRange(3, 3);
//	rightKidneyFilter->SetExtractionModeToLargestRegion();
//	rightKidneyFilter->SetInputData(niiReader->GetOutput());
//	rightKidneyFilter->Update();
//
//	vtkSmartPointer<vtkMarchingCubes> rightKidneyMC = vtkSmartPointer<vtkMarchingCubes>::New();
//	rightKidneyMC->SetInputConnection(rightKidneyFilter->GetOutputPort());
//	rightKidneyMC->SetValue(0, 1);
//	rightKidneyMC->Update();
//
//	vtkSmartPointer<vtkFillHolesFilter> rightKidneyfillHoles = vtkSmartPointer<vtkFillHolesFilter>::New();
//	rightKidneyfillHoles->SetHoleSize(1e3);
//	rightKidneyfillHoles->SetInputData(rightKidneyMC->GetOutput());
//	rightKidneyfillHoles->Update();
//	
//	vtkSmartPointer<vtkSmoothPolyDataFilter> rightKidneysmoothFilter = vtkSmartPointer<vtkSmoothPolyDataFilter>::New();
//	rightKidneysmoothFilter->SetInputConnection(rightKidneyfillHoles->GetOutputPort());
//	rightKidneysmoothFilter->SetNumberOfIterations(15);
//	rightKidneysmoothFilter->SetRelaxationFactor(0.1);
//	rightKidneysmoothFilter->FeatureEdgeSmoothingOff();
//	rightKidneysmoothFilter->BoundarySmoothingOn();
//	rightKidneysmoothFilter->Update();
//
//	vtkSmartPointer<vtkPolyDataNormals> rightKidneynormalGenerator = vtkSmartPointer<vtkPolyDataNormals>::New();
//	rightKidneynormalGenerator->SetInputConnection(rightKidneysmoothFilter->GetOutputPort());
//	rightKidneynormalGenerator->ComputePointNormalsOn();
//	rightKidneynormalGenerator->ComputeCellNormalsOn();
//	rightKidneynormalGenerator->Update();
//
//	vtkSmartPointer<vtkPolyDataMapper> rightKidneyMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
//	rightKidneyMapper->SetInputConnection(rightKidneynormalGenerator->GetOutputPort());
//	rightKidneyMapper->ScalarVisibilityOff();
//
//	vtkSmartPointer<vtkProperty> rightKidneyProp = vtkSmartPointer<vtkProperty>::New();
//	rightKidneyProp->SetColor(rightKidneyColor);
//	rightKidneyProp->SetOpacity(0.5);
//
//	vtkSmartPointer<vtkActor> rightKidneyActor = vtkSmartPointer<vtkActor>::New();
//	rightKidneyActor->SetMapper(rightKidneyMapper);
//	rightKidneyActor->SetProperty(rightKidneyProp);
//
//	// 提取肝脏肿瘤
//	vtkSmartPointer<vtkImageConnectivityFilter> livertumorFilter = vtkSmartPointer<vtkImageConnectivityFilter>::New();
//	livertumorFilter->SetScalarRange(4, 4);
//	livertumorFilter->SetInputData(niiReader->GetOutput());
//	livertumorFilter->Update();
//
//	vtkSmartPointer<vtkMarchingCubes> livertumorMC = vtkSmartPointer<vtkMarchingCubes>::New();
//	livertumorMC->SetInputConnection(livertumorFilter->GetOutputPort());
//	livertumorMC->SetValue(0, 1);
//	livertumorMC->Update();
//
//	vtkSmartPointer<vtkSmoothPolyDataFilter> livertumorsmoothFilter = vtkSmartPointer<vtkSmoothPolyDataFilter>::New();
//	livertumorsmoothFilter->SetInputConnection(livertumorMC->GetOutputPort());
//	livertumorsmoothFilter->SetNumberOfIterations(15);
//	livertumorsmoothFilter->SetRelaxationFactor(0.1);
//	livertumorsmoothFilter->FeatureEdgeSmoothingOff();
//	livertumorsmoothFilter->BoundarySmoothingOn();
//	livertumorsmoothFilter->Update();
//
//	vtkSmartPointer<vtkPolyDataNormals> livertumornormalGenerator = vtkSmartPointer<vtkPolyDataNormals>::New();
//	livertumornormalGenerator->SetInputConnection(livertumorsmoothFilter->GetOutputPort());
//	livertumornormalGenerator->ComputePointNormalsOn();
//	livertumornormalGenerator->ComputeCellNormalsOn();
//	livertumornormalGenerator->Update();
//
//	vtkSmartPointer<vtkPolyDataMapper> livertumorMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
//	livertumorMapper->SetInputConnection(livertumornormalGenerator->GetOutputPort());
//	livertumorMapper->ScalarVisibilityOff();
//
//	vtkSmartPointer<vtkProperty> livertumorProp = vtkSmartPointer<vtkProperty>::New();
//	livertumorProp->SetColor(livertumorColor);
//
//	vtkSmartPointer<vtkActor> livertumorActor = vtkSmartPointer<vtkActor>::New();
//	livertumorActor->SetMapper(livertumorMapper);
//	livertumorActor->SetProperty(livertumorProp);
//
//	// 提取肝脏肿瘤包围盒
//	vtkSmartPointer<vtkPolyDataConnectivityFilter> connectivityFilter = vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
//	connectivityFilter->SetInputData(livertumorMC->GetOutput());
//	connectivityFilter->SetExtractionModeToAllRegions();
//	connectivityFilter->Update();
//
//	vector<vtkSmartPointer<vtkActor>> obbtreeActors;
//	for (int i = 0; i < connectivityFilter->GetNumberOfExtractedRegions(); i++)
//	{
//		vtkSmartPointer<vtkPolyDataConnectivityFilter> connectivityFilter = vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
//		connectivityFilter->SetInputData(livertumorMC->GetOutput());
//		connectivityFilter->SetExtractionModeToSpecifiedRegions();
//		connectivityFilter->AddSpecifiedRegion(i);
//		connectivityFilter->Update();
//
//		vtkSmartPointer<vtkOBBTree> obbTree = vtkSmartPointer<vtkOBBTree>::New();
//		obbTree->SetDataSet(connectivityFilter->GetOutput());
//		obbTree->BuildLocator();
//
//		vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
//		obbTree->GenerateRepresentation(0, polydata);
//
//		double *bbox = polydata->GetBounds();
//		cout << "肿瘤编号: " << i + 1 << " 三维包围盒: ";
//		for (int i = 0; i < 6; i++) cout << bbox[i] << ", ";
//		cout << endl;
//
//		vtkSmartPointer<vtkPolyDataMapper> obbtreeMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
//		obbtreeMapper->SetInputData(polydata);
//
//		vtkSmartPointer<vtkActor> obbtreeActor = vtkSmartPointer<vtkActor>::New();
//		obbtreeActor->SetMapper(obbtreeMapper);
//		obbtreeActor->GetProperty()->SetRepresentationToWireframe();
//
//		obbtreeActors.push_back(obbtreeActor);
//	}
//	
//
//	// 四个视角的窗口位置
//	double viewport1[] = {0, 0.505, 0.495, 1};
//	double viewport2[] = {0.505, 0.505, 1, 1};
//	double viewport3[] = {0, 0, 0.495, 0.495};
//	double viewport4[] = {0.505, 0, 1, 0.495};
//
//	vtkSmartPointer<vtkRenderer> renderer1 = vtkSmartPointer<vtkRenderer>::New();
//	renderer1->AddActor(liverActor);
//	renderer1->AddActor(leftKidneyActor);
//	renderer1->AddActor(rightKidneyActor);
//	renderer1->AddActor(livertumorActor);
//	renderer1->SetBackground(0.1, 0.2, 0.4);
//	renderer1->SetBackground2(0.2, 0.6, 0.8);
//	renderer1->SetGradientBackground(true);
//	renderer1->SetViewport(viewport1);
//
//	vtkSmartPointer<vtkRenderer> renderer2 = vtkSmartPointer<vtkRenderer>::New();
//	renderer2->AddActor(liverActor);
//	renderer2->AddActor(livertumorActor);
//	renderer2->SetActiveCamera(renderer1->GetActiveCamera());
//	renderer2->ResetCamera();
//	renderer2->SetBackground(0.1, 0.2, 0.4);
//	renderer2->SetBackground2(0.2, 0.6, 0.8);
//	renderer2->SetGradientBackground(true);
//	renderer2->SetViewport(viewport2);
//
//	vtkSmartPointer<vtkRenderer> renderer3 = vtkSmartPointer<vtkRenderer>::New();
//	renderer3->AddActor(leftKidneyActor);
//	renderer3->AddActor(rightKidneyActor);
//	renderer3->SetActiveCamera(renderer1->GetActiveCamera());
//	renderer3->ResetCamera();
//	renderer3->SetBackground(0.1, 0.2, 0.4);
//	renderer3->SetBackground2(0.2, 0.6, 0.8);
//	renderer3->SetGradientBackground(true);
//	renderer3->SetViewport(viewport3);
//
//	vtkSmartPointer<vtkRenderer> renderer4 = vtkSmartPointer<vtkRenderer>::New();
//	renderer4->AddActor(livertumorActor);
//	for (int i = 0; i < obbtreeActors.size(); i++)renderer4->AddActor(obbtreeActors[i]);
//	renderer4->SetActiveCamera(renderer1->GetActiveCamera());
//	renderer4->ResetCamera();
//	renderer4->SetBackground(0.1, 0.2, 0.4);
//	renderer4->SetBackground2(0.2, 0.6, 0.8);
//	renderer4->SetGradientBackground(true);
//	renderer4->SetViewport(viewport4);
//
//	vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
//	renWin->AddRenderer(renderer1);
//	renWin->AddRenderer(renderer2);
//	renWin->AddRenderer(renderer3);
//	renWin->AddRenderer(renderer4);
//	renWin->SetSize(800, 800);
//
//	vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
//	iren->SetRenderWindow(renWin);
//
//	vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
//	iren->SetInteractorStyle(style);
//	iren->Initialize();
//	iren->Start();
//
//	return EXIT_SUCCESS;
//}