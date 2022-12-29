/*//////////////////////////////////////////////////////////////////
// Author: Dale Hamilton
// Copyright 2014 by SEM LLC
//
// under contract with the Wildfire Management - Research, Development & Applications Group
//////////////////////////////////////////////////////////////////*/
#include "StdAfx.h"
#include "Poly2Raster.h"
#include "ParamPoly2Raster.h"
#include "ArcGISUtil.h"


using namespace FreqTool;

using namespace System;
using namespace System::IO;
using namespace System::Runtime::InteropServices;
using namespace ESRI::ArcGIS::Geoprocessor;
using namespace ESRI::ArcGIS::Geoprocessing;
using namespace ESRI::ArcGIS::DataManagementTools;
using namespace ESRI::ArcGIS::AnalysisTools;
using namespace ESRI::ArcGIS::ConversionTools;
using namespace ESRI::ArcGIS::SpatialAnalystTools;

//using namespace System::IO;
//using namespace System::Text;
//using namespace System::Diagnostics;
//using namespace Microsoft::Win32;
//using namespace System::Windows::Forms;
//using namespace System::Collections;
//using namespace FF_Wrapper;
//using namespace System::Data::OleDb;
//using namespace std;
//using namespace MFbatEngine;


#ifdef MessageBox
#undef MessageBox
#endif

Poly2Raster::Poly2Raster(IntPtr pApp)
{
	InitializeComponent();

	void * vapp = (void *)pApp;
	m_piApp = pApp;
	m_pApp = (IApplication *)vapp;

}

System::Void Poly2Raster::Poly2Raster_Load(System::Object^  sender, System::EventArgs^  e) 
{
	// populate the combo boxes.
	IMxDocumentPtr ipMxDocument;
	IDocumentPtr ipDocument;
	m_pApp->get_Document(&ipDocument);

	ipMxDocument = ipDocument;
	IMapPtr ipMap;
	ipMxDocument->get_FocusMap(&ipMap);

	this->cbMask->Items->Add("No Mask");
	this->cbMask->Text = "No Mask";
	long layerCount;
	ipMap->get_LayerCount(&layerCount);
	bool bFlmFound = false;
	for (int i = 0; i < layerCount; i++)
	{

		ILayerPtr pLayer;
		ipMap->get_Layer(i,&pLayer);
		BSTR name = ::SysAllocStringLen(L"", 0);
		pLayer->get_Name(&name);
		String^ sName = Marshal::PtrToStringBSTR(static_cast<IntPtr>(name));

		// Load raster names into raster comboboxes
		IRasterLayerPtr pRasterLayer;
		pRasterLayer = pLayer;
		if (pRasterLayer != 0)
		{
			this->cbMask->Items->Add(sName);
		}

		// Load feature names into feature comboboxes
		IFeatureLayerPtr pFeatureLayer;
		pFeatureLayer = pLayer;

		if (pFeatureLayer != 0)
		{
			this->cbInput->Items->Add(sName);
		}

	}

}

System::Void Poly2Raster::ckFilter_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
{
	cbFilterAttribute->Enabled = this->ckFilter->Checked;
	nudFilterYearFrom->Enabled = this->ckFilter->Checked;
	nudFilterYearTo->Enabled = this->ckFilter->Checked;
	if (!this->ckFilter->Checked)
		cbFilterAttribute->Text = "";
}


System::Void Poly2Raster::cbInput_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
{

	String ^sRasterName = this->cbInput->Text;
	IMxDocumentPtr ipMxDocument;
	IDocumentPtr ipDocument;
	m_pApp->get_Document(&ipDocument);

	ipMxDocument = ipDocument;
	IMapPtr ipMap;
	ipMxDocument->get_FocusMap(&ipMap);

	// get raster from layer
	ILayerPtr pLayer;
	ITableFieldsPtr pTableFields;
	String^ sLayerName;

	long layerCount;
	ipMap->get_LayerCount(&layerCount);
	for (int i = 0; i < layerCount; i++)
	{
		ipMap->get_Layer(i, &pLayer);
		BSTR name = ::SysAllocStringLen(L"", 0);
		pLayer->get_Name(&name);
		String^ sName = Marshal::PtrToStringBSTR(static_cast<IntPtr>(name));

		if (sName->Equals(sRasterName))
		{
			pTableFields = (ITableFieldsPtr)pLayer;
			sLayerName = sName;
			break;
		}
	}

	// Put attributes on the Attribute Cell combobox
	long lFieldCount;
	pTableFields->get_FieldCount(&lFieldCount);
	cbFilterAttribute->Items->Clear();

	for (int i = 0; i <= lFieldCount - 1; i++)
	{
		IFieldPtr pField;
		pTableFields->get_Field(i, &pField);
		BSTR name = ::SysAllocStringLen(L"", 0);
		pField->get_Name(&name);
		String^ sFieldName = Marshal::PtrToStringBSTR(static_cast<IntPtr>(name));

		esriFieldType eFieldType;
		pField->get_Type(&eFieldType);

		bool bFieldIsNumeric = false;
		if (eFieldType == esriFieldTypeSmallInteger ||
			eFieldType == esriFieldTypeInteger ||
			eFieldType == esriFieldTypeSingle ||
			eFieldType == esriFieldTypeDouble ||
			eFieldType == esriFieldTypeString)

		{
			bFieldIsNumeric = true;
		}

		if (bFieldIsNumeric && 
			!sFieldName->Equals("COUNT"))
		{
			cbFilterAttribute->Items->Add(sFieldName);
			if (sFieldName->Equals("VALUE"))
				cbFilterAttribute->Text= sFieldName;

		}


	}

}

System::Void Poly2Raster::btnOutputRaster_Click(System::Object^  sender, System::EventArgs^  e) 
{

	::OleInitialize(NULL);
	IGxDialogPtr ipGxDialog(CLSID_GxDialog);
	IGxObjectPtr ipGxObject;
	IGxObjectFilterPtr ipGxObjectFilterFeatureClasses(CLSID_RasterFormatGridFilter);
	String ^ pathName;
	String ^ fileName;

	BSTR title = ::SysAllocStringLen(L"Browse to select the BpS layer...", 0);
	VARIANT_BOOL result = VARIANT_FALSE;

	ipGxDialog->putref_ObjectFilter(ipGxObjectFilterFeatureClasses);
	ipGxDialog->put_Title(title);
	BSTR openBtnCaption = ::SysAllocStringLen(L"Open", 0);
	ipGxDialog->put_ButtonCaption(openBtnCaption);


	try
	{
		ipGxDialog->DoModalSave(0, &result);
	}
	catch (...)
	{
		::OleUninitialize();
		return;
	}
	BSTR pathBSTR;
	BSTR fileNameBSTR;

	if (result == VARIANT_TRUE)
	{
		this->Refresh();
		ipGxDialog->get_FinalLocation(& ipGxObject);
		ipGxObject->get_FullName(& pathBSTR);
		ipGxDialog->get_Name(&fileNameBSTR);

		pathName = Marshal::PtrToStringBSTR(static_cast<IntPtr>(pathBSTR));
		fileName = Marshal::PtrToStringBSTR(static_cast<IntPtr>(fileNameBSTR));

		tbOutputRaster->Text = pathName + "\\" + fileName;
	}
	if (result == VARIANT_FALSE)
	{
		tbOutputRaster->Text = 	L"";
	}
	ipGxDialog = 0; 
	::OleUninitialize();
}


System::Void Poly2Raster::btnRun_Click(System::Object^  sender, System::EventArgs^  e) 
{
	ArcGISUtil ^pArcUtil = gcnew ArcGISUtil(m_piApp, "Fire Frequency Tool"); 

	this->Cursor = Cursors::WaitCursor;
	String^ sInput = cbInput->Text;
	ParamPoly2Raster ^param = 
		gcnew ParamPoly2Raster(pArcUtil, cbInput->Text, ckFilter->Checked, 
								cbFilterAttribute->Text,
								System::Convert::ToInt32(nudFilterYearFrom->Value), 
								System::Convert::ToInt32(nudFilterYearTo->Value),
								cbMask->Text, tbOutputRaster->Text);

	Geoprocessor ^gp = gcnew Geoprocessor();
	IGeoProcessorResult ^gpResult;
	String ^ msg = "starting.";
	try
	{
		// make sure project folder is empty and create temp folder
		String ^sProjectFolder = param->getOutputFolderPath();
		if (!pArcUtil->checkAndCreateFolders(sProjectFolder))
			return;
		String ^sTempFolder = sProjectFolder + "\\temp";
		String ^sFilterLayerName;

		param->writeParamFile();

		// if temporal filter is used, select fires within that date range
		if (param->hasFilter())
		{
			msg = "creating filter layer";
			int iLayerNum = 1;
			sFilterLayerName = param->getInputName() + "_f" + Convert::ToString(iLayerNum);
			//arcpy.MakeFeatureLayer_management("FireHistPolyClw_20131230","FireHistPolyClw_20131230_Lay","#","#","FID FID VISIBLE NONE;Shape Shape VISIBLE NONE;FIRE_NAME FIRE_NAME VISIBLE NONE;FIRE_DAY FIRE_DAY VISIBLE NONE;FIRE_MONTH FIRE_MONTH VISIBLE NONE;FIRE_YEAR FIRE_YEAR VISIBLE NONE;DATA_SOURC DATA_SOURC VISIBLE NONE;COST COST VISIBLE NONE;LOCAL_NUMB LOCAL_NUMB VISIBLE NONE;CAUSE CAUSE VISIBLE NONE;REPORTED_A REPORTED_A VISIBLE NONE;SIZE_CLASS SIZE_CLASS VISIBLE NONE;AGENCY AGENCY VISIBLE NONE;COMMENTS COMMENTS VISIBLE NONE;ARC_ACRES ARC_ACRES VISIBLE NONE;NIFMID_CN NIFMID_CN VISIBLE NONE;NIFMID_LNK NIFMID_LNK VISIBLE NONE;STRATEGY STRATEGY VISIBLE NONE;DECADE DECADE VISIBLE NONE;SHAPE_AREA SHAPE_AREA VISIBLE NONE;SHAPE_LEN SHAPE_LEN VISIBLE NONE")
			MakeFeatureLayer ^makeFeatureLayer = gcnew MakeFeatureLayer();
			makeFeatureLayer->in_features =  param->getInputPath();
			makeFeatureLayer->out_layer = sFilterLayerName;
			try
			{
				gpResult = (IGeoProcessorResult ^)gp->Execute(makeFeatureLayer, nullptr);
			}
			catch (Exception ^e)
			{
				// There may already be a layer with this name, bump the count up and try again.
				sFilterLayerName = param->getInputName() + "_f" + Convert::ToString(++iLayerNum);
				makeFeatureLayer->out_layer = sFilterLayerName;
				gpResult = (IGeoProcessorResult ^)gp->Execute(makeFeatureLayer, nullptr);
			}

			

			msg = "selecting by filter.";
			// arcpy.SelectLayerByAttribute_management("FireHistPolyClw_20131230","NEW_SELECTION",""""FIRE_YEAR" >= '1907' AND "FIRE_YEAR" <= '1983'""")
			SelectLayerByAttribute ^selLayerByAttrib = gcnew SelectLayerByAttribute();
//			selLayerByAttrib->in_layer_or_view = param->getInputPath();
			selLayerByAttrib->in_layer_or_view = sFilterLayerName;
			selLayerByAttrib->selection_type = "NEW_SELECTION";
			String ^sWhereClause = "\"" + param->getYearAttrib() + "\" >= '" + param->getYearFrom() + "' and \"" + param->getYearAttrib() + "\" <= '" + param->getYearTo() + "\'";
			selLayerByAttrib->where_clause = sWhereClause;
			try
			{

				gpResult = (IGeoProcessorResult ^)gp->Execute(selLayerByAttrib, nullptr);
			}
			catch (Exception ^e)
			{
				String ^sWhereClause = "\"" + param->getYearAttrib() + "\" >= " + param->getYearFrom() + " and \"" + param->getYearAttrib() + "\" <= " + param->getYearTo();
				selLayerByAttrib->where_clause = sWhereClause;
				gpResult = (IGeoProcessorResult ^)gp->Execute(selLayerByAttrib, nullptr);

			}

		}

		// Use Feature to poly to create spaghetti fire occurrence boundaries - Feature to Polygon
		msg = "generating fire occurrence polygons.";
		FeatureToPolygon ^feat2Poly = gcnew FeatureToPolygon();
		if (param->hasFilter())
			feat2Poly->in_features = sFilterLayerName;
		else
			feat2Poly->in_features = param->getInputPath();
		feat2Poly->out_feature_class = sTempFolder + "\\seg.shp";
		gpResult = (IGeoProcessorResult ^)gp->Execute(feat2Poly, nullptr);

	
		// Get centroid of each poly - Feature to point 
		msg = "generating centroids.";
		FeatureToPoint ^feat2Point = gcnew FeatureToPoint();
		feat2Point->in_features = sTempFolder + "\\seg.shp";
		feat2Point->out_feature_class = sTempFolder + "\\seg_cent.shp";
		feat2Point->point_location = "INSIDE";
		gpResult = (IGeoProcessorResult ^)gp->Execute(feat2Point, nullptr);

		// Check how many fires overlap each poly centroid – Spatial Join (Analysis Tools -> Overlay) 
		msg = "checking how many fires occurred for each centroid.";
		ESRI::ArcGIS::AnalysisTools::SpatialJoin ^spatialJoin = 
			gcnew ESRI::ArcGIS::AnalysisTools::SpatialJoin();
		spatialJoin->target_features = sTempFolder + "\\seg_cent.shp";
		spatialJoin->join_features = param->getInputPath();
		spatialJoin->out_feature_class = sTempFolder + "\\seg_join.shp";
		spatialJoin->match_option = "WITHIN";
		spatialJoin->join_type = "KEEP_ALL";
		spatialJoin->join_operation = "JOIN_ONE_TO_ONE";
		gpResult = (IGeoProcessorResult ^)gp->Execute(spatialJoin, nullptr);

		// join Join_Count column from seg_join pt shapefile back to seg shapfile.
		msg = "joining Join_Count column.";
		ESRI::ArcGIS::DataManagementTools::JoinField ^jf = 
			gcnew ESRI::ArcGIS::DataManagementTools::JoinField();
		jf->in_data = sTempFolder + "\\seg.shp";
		jf->in_field = "FID";
		jf->join_table = sTempFolder + "\\seg_join.shp";
		jf->join_field = "FID";
		jf->fields = "Join_Count";
		gpResult = (IGeoProcessorResult ^)gp->Execute(jf, nullptr);



		if (param->hasMask())
		{

			// set environment variables
			String ^sMaskPath = param->getMaskPath();
			msg = "setting environment variables.";
			gp->SetEnvironmentValue("cellSize", sMaskPath);
			gp->SetEnvironmentValue("mask", sMaskPath);
			gp->SetEnvironmentValue("extent", sMaskPath);
			gp->SetEnvironmentValue("snapRaster", sMaskPath);
			msg = "setting environment variables: qualifiedFieldNames.";
//			gp->SetEnvironmentValue("qualifiedFieldNames ", "FALSE");
		}

		// Convert segmented polys with joined fire count (Join_Count) to a raster
		msg = "converting segmented polys to raster.";
		ESRI::ArcGIS::ConversionTools::PolygonToRaster ^poly2Ras = 
			gcnew ESRI::ArcGIS::ConversionTools::PolygonToRaster();
		poly2Ras->in_features = sTempFolder + "\\seg.shp";
		poly2Ras->value_field = "Join_Count";
		poly2Ras->out_rasterdataset = sTempFolder + "\\fire";
		poly2Ras->cell_assignment = "CELL_CENTER";
		gpResult = (IGeoProcessorResult ^)gp->Execute(poly2Ras, nullptr);


		// Convert no fire pixels to 0
		msg = "Convert no fire pixels to 0";
		ESRI::ArcGIS::SpatialAnalystTools::RasterCalculator ^rasCalc = gcnew ESRI::ArcGIS::SpatialAnalystTools::RasterCalculator();
		rasCalc->output_raster = param->getOutputPath();
		rasCalc->expression = "Con(IsNull(\"fire\"), 0, \"fire\")";
		gpResult = (IGeoProcessorResult ^)gp->Execute(rasCalc, nullptr);

		// cleanup intermediate layers
		pArcUtil->deleteLayer("seg_join");
		pArcUtil->deleteLayer("seg_cent");
		pArcUtil->deleteLayer("seg");
		pArcUtil->deleteLayer(sFilterLayerName);
		pArcUtil->deleteLayer("fire");

	}
	catch (Exception ^e)
	{
		MessageBox::Show("Error calculating Fire Occurrence while " + msg, 
			"Fire Frequency Tool", MessageBoxButtons::OK, MessageBoxIcon::Error);

	}


}
