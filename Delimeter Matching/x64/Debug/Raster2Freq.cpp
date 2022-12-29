/*//////////////////////////////////////////////////////////////////
// Author: Dale Hamilton
// Copyright 2014 by SEM LLC
//
// under contract with the Wildfire Management - Research, Development & Applications Group
//////////////////////////////////////////////////////////////////*/

#include "StdAfx.h"
#include "Raster2Freq.h"
#include "ParamRaster2Freq.h"

#include "ArcGISUtil.h"
#include "ParamConjoin.h"

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

using namespace System::Diagnostics;
using namespace Microsoft::Win32;
using namespace System::Windows::Forms;
using namespace System::Collections;
using namespace System::Drawing;
using namespace System::Data::OleDb;


using namespace AccessUtilitiesNet2;


Raster2Freq::Raster2Freq(IntPtr pApp)
{
	InitializeComponent();

	void * vapp = (void *)pApp;
	m_piApp = pApp;
	m_pApp = (IApplication *)vapp;

}


System::Void Raster2Freq::Raster2Freq_Load(System::Object^  sender, System::EventArgs^  e) 
{
	// populate the combo boxes.
	IMxDocumentPtr ipMxDocument;
	IDocumentPtr ipDocument;
	m_pApp->get_Document(&ipDocument);

	ipMxDocument = ipDocument;
	IMapPtr ipMap;
	ipMxDocument->get_FocusMap(&ipMap);

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
			this->dgvcRaster->Items->Add(sName);
			this->cbLandscapeGrid->Items->Add(sName);
			this->cbBpsGrid->Items->Add(sName);
		}
	}

	// Load refcon combobox
	String ^sRefconPath = GetRefConPath();
	PopulateAccessTableList(this->cbRefCon, sRefconPath, false);

}

System::String^ Raster2Freq::GetRefConPath()
{
	try
	{
		bool pathExists = false;
		String ^sRefConPath;

		// figure out where the fofemmt database is located.
		String ^sAsmLoc = System::Reflection::Assembly::GetExecutingAssembly()->Location;
		int binLoc = sAsmLoc->IndexOf("\\bin\\FreqTool.dll");

		if (binLoc >= 0)
		{

		sAsmLoc = sAsmLoc->Remove(binLoc, sAsmLoc->Length - binLoc);
		sRefConPath = sAsmLoc + "\\Reference Conditions Database\\RefCon.mdb";
		pathExists = System::IO::File::Exists(sRefConPath);

		}

		if (!pathExists)
		{
			sAsmLoc = System::Reflection::Assembly::GetExecutingAssembly()->Location;
			binLoc = sAsmLoc->IndexOf("\\Debug\\FreqTool.dll");

			sAsmLoc = sAsmLoc->Remove(binLoc, sAsmLoc->Length - binLoc);
			sRefConPath = sAsmLoc + "\\Reference Conditions Database\\RefCon.mdb";

			pathExists = System::IO::File::Exists(sRefConPath);
			if (!pathExists)
			{
				MessageBox::Show("Could not locate Reference Condition database.", "Frequency Tool",MessageBoxButtons::OK, MessageBoxIcon::Error);
				return "";
			}
		}
		return sRefConPath;
	}
	catch (Exception ^e)
	{
		MessageBox::Show("Could not locate Reference Condition database. \n" + e->Message, "Frequency Tool", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}

}


System::Void Raster2Freq::cbLandscapeGrid_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
{
//	ArcGISUtil ^pArcUtil = gcnew ArcGISUtil(m_piApp, "Fire Frequency Tool"); 
	setAttributeCB(this->cbLandscapeGrid->Text, cbLandscapeAttrib); 

}

System::Void Raster2Freq::cbBpsGrid_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
{
	//	ArcGISUtil ^pArcUtil = gcnew ArcGISUtil(m_piApp, "Fire Frequency Tool"); 
	setAttributeCB(this->cbBpsGrid->Text, cbBpsAttrib); 

}


System::Void Raster2Freq::setAttributeCB(String^ sRasterName, System::Windows::Forms::ComboBox^  cbAttrib) 
{
//	String ^sRasterName = this->cbLandscapeGrid->Text;

	ArcGISUtil ^pArcUtil = gcnew ArcGISUtil(m_piApp, "Fire Frequency Tool"); 


	if (!pArcUtil->RasterIsInteger(sRasterName))  // Floating Point grids don't have attribute tables
	{
		cbAttrib->Items->Add("VALUE");
	}
	else
	{
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

		// Put attributes on the Attribute combobox
		long lFieldCount;
		pTableFields->get_FieldCount(&lFieldCount);
		cbAttrib->Items->Clear();

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
				eFieldType == esriFieldTypeString )
			{
				bFieldIsNumeric = true;
			}

			if (bFieldIsNumeric && 
				!sFieldName->Equals("COUNT"))
			{
				cbAttrib->Items->Add(sFieldName);
				if (sFieldName->Equals("VALUE"))
					cbAttrib->Text = sFieldName;

			}
		}
	}

}



System::Void Raster2Freq::btnRun_Click(System::Object^  sender, System::EventArgs^  e) 
{
	IMxDocumentPtr ipMxDocument;
	IDocumentPtr ipDocument;
	m_pApp->get_Document(&ipDocument);

	ipMxDocument = ipDocument;
	IContentsViewPtr pCurrentContentsView;
	ipMxDocument->get_CurrentContentsView(&pCurrentContentsView);

	ArcGISUtil ^pArcUtil = gcnew ArcGISUtil(m_piApp, "Fire Frequency Tool"); 

	this->Cursor = Cursors::WaitCursor;

	// load parameters
	int iPerLen = System::Decimal::ToInt32(nudHistPeriod->Value);
	
	ParamRaster2Freq ^param = gcnew ParamRaster2Freq(pArcUtil, iPerLen, 
		cbLandscapeGrid->Text, cbLandscapeAttrib->Text,
		cbBpsGrid->Text, cbBpsAttrib->Text,
		cbRefCon->Text, tbOutputFolder->Text);

	int iRasterCount = this->dgvRasters->Rows->Count;
	if (iRasterCount == 1)
	{
		MessageBox::Show("Please specify input fire occurrence rasters.", "Fire Frequency Tool", MessageBoxButtons::OK,MessageBoxIcon::Error);

		return;
	}

	try
	{
		for (int i=0;i<iRasterCount-1;i++)
		{
			String ^sRasterName = this->dgvRasters->Rows[i]->Cells["dgvcRaster"]->Value->ToString();
			param->addRasterName(sRasterName);
		}

	}
	catch (...)
	{
		MessageBox::Show("Please specify rasters.", "Fire Frequency Tool", MessageBoxButtons::OK,MessageBoxIcon::Error);
		return;
	}

	Geoprocessor ^gp = gcnew Geoprocessor();
	IGeoProcessorResult ^gpResult;

	// make sure project folder is empty and create temp folder
	String ^sProjectFolder = param->getOutputPath();
	if (!pArcUtil->checkAndCreateFolders(sProjectFolder))
		return;
	String ^sTempFolder = sProjectFolder + "\\temp";
	param->writeParamFile();

	if (!addRasters(param))
		return;

	// combine landscape, BpS and Fire Occurrence
	if (!combineLndscpBpsFireOcc(param))
		return;

	// combine landscape and BpS
	if (!combineLndscpBps(param))
		return;

	// Create personal database
	if (!createPersonalGdb(param))
		return; 

	if (!loadRefCon(param))
		return; 


	// Copy attribute tables to geodatabase
	if (!exportRasterTableToGdb(param, "LndBpsFo"))
		return; 
	if (!exportRasterTableToGdb(param, "LndBps"))
		return; 
	if (!exportRasterTableToGdb(param, param->getBpsRasterName()))
		return; 
	if (!exportRasterTableToGdb(param, param->getLndscpRasterName()))
		return; 

	if (!connectToPersonalGdb(param))
		return;

	// Create Frequency table and calculate metrics
	if (!createRefFreq(param))
		return; 
	if (!createLndBpsFreq(param))
		return; 

	// Join Frequency table back to LndBps grid
	KillAllAccess();
	Sleep(5000);			// give KillAllAccess time to complete
	if (!joinFreq(param))
		return; 

	// Reclass on metrics
	if (!reclassLndBps(param, "MeanFireInterval", "MFI"))
		return; 
	if (!reclassLndBps(param, "Frequency", "Frequency"))
		return; 
//	if (!reclassLndBps(param, "MFIPctDiff", "MFIPctDiff"))
	//	return; 

	pArcUtil->deleteLayer("LndBpsFo");

	ipMxDocument->putref_CurrentContentsView(pCurrentContentsView);

}


bool Raster2Freq::loadRefCon(ParamRaster2Freq ^param)
{
	String ^sMsg = "loading refcon table.";
	try 
	{
		// Open connection to RefCon database
		String ^sRefconPath = GetRefConPath();
		String ^sConnectionString = "Provider=Microsoft.Jet.OLEDB.4.0; Data Source=" + sRefconPath;
		OleDbConnection^ pRefConConnection = gcnew OleDbConnection(sConnectionString);
		pRefConConnection->Open();

		OleDbCommand ^pRefConCommand = pRefConConnection->CreateCommand();
		pRefConCommand->CommandType = CommandType::Text;

		// Copy RefCon
		// SELECT *
		// 		into LFnat_100108 in 'C:\Data\Lochsa\Outputs\freq6\freq.mdb'
		//		from LFnat_100108;

		String^ sQuery;
		sQuery = "SELECT * " +
			"INTO " + param->getRefConTableName() + " in '" + param->getOutputPath() + "\\freq.mdb' " +
			"FROM " + param->getRefConTableName();

		pRefConCommand->CommandText = sQuery;
		pRefConCommand->ExecuteNonQuery();
		pRefConConnection->Close();


		return true;

	}
	catch (...)
	{
		MessageBox::Show("Error " + sMsg, 
			"Fire Frequency Tool", MessageBoxButtons::OK, MessageBoxIcon::Error);
		return false;
	}

}


bool Raster2Freq::createRefFreq(ParamRaster2Freq ^param)
{
	String ^sMsg = "creating reference frequency table.";
	try 
	{
		// Create table of reference frequencies for BpS in this study area.
		OleDbCommand ^pCommand = param->getFreqDbConn()->CreateCommand();
//		MessageBox::Show("Set Command type", "Fire Frequency Tool", MessageBoxButtons::OK, MessageBoxIcon::Information);
		pCommand->CommandType = CommandType::Text;

		// SELECT Refresh_110825.Frequency
		// FROM BpS INNER JOIN Refresh_110825 ON BpS.GROUPMODEL = Refresh_110825.BpS_Model ;

//		MessageBox::Show("setup query string","Fire Frequency Tool", MessageBoxButtons::OK, MessageBoxIcon::Information);

		String ^sQuery = 
			"SELECT Frequency " +
			"INTO RefFreq " +
			"from " + param->getBpsRasterName() + " INNER JOIN " + param->getRefConTableName() + " " +
			"ON "  + param->getBpsRasterName() + "." + param->getBpsAttribName() + " = " +
			param->getRefConTableName() + ".BpS_Model"; 

//		MessageBox::Show("set command text","Fire Frequency Tool", MessageBoxButtons::OK, MessageBoxIcon::Information);

		pCommand->CommandText = sQuery;
//		MessageBox::Show("execute query","Fire Frequency Tool", MessageBoxButtons::OK, MessageBoxIcon::Information);
		pCommand->ExecuteNonQuery();
//		MessageBox::Show("done","Fire Frequency Tool", MessageBoxButtons::OK, MessageBoxIcon::Information);


		return true;
	}
	catch (...)
	{
		MessageBox::Show("Error " + sMsg, 
			"Fire Frequency Tool", MessageBoxButtons::OK, MessageBoxIcon::Error);
		return false;
	}

}


bool Raster2Freq::createLndBpsFreq(ParamRaster2Freq ^param)
{
	String ^sMsg = "creating Frequency metrics table.";
	try 
	{
		OleDbCommand ^pCommand = param->getFreqDbConn()->CreateCommand();
		pCommand->CommandType = CommandType::Text;

		// Create table
		/*
		SELECT LndBps.VALUE_, LndBps.HUC12, LndBps.BPS, LndBps.COUNT_ AS BpsSize, 
		    sum(LndBpsFo.COUNT_ * LndBpsFo.FO) as FireOcc,
			0 AS AreaBurned,
			0 AS AreaBurnedPct,
			round(0,1) AS MeanAnnualBurn,
			0 AS MeanFireInterval,
			round(0,1) AS MeanFireOccurrances,
			0 AS Frequency
		INTO LndBpsFreq 
		FROM LndBpsFo INNER JOIN LndBps ON (LndBpsFo.BPS = LndBps.BPS) AND (LndBpsFo.HUC12 = LndBps.HUC12)
		group by LndBps.VALUE_, LndBps.HUC12, LndBps.BPS, LndBps.COUNT_
		*/


		String^ sQuery;
			sQuery = "SELECT LndBps.VALUE_, LndBps." + param->getLndscpRasterName() + ", " +
					 "       LndBps." + param->getBpsRasterName() +  ", " +
					 "       LndBps.COUNT_ AS BpsSize, sum(LndBpsFo.COUNT_ * LndBpsFo.FO) as AreaBurned, " +
					 "       0 AS AreaBurnedPct, round(0,1) AS MeanAnnualBurn, " +
					 "       0 AS MeanFireInterval, round(0,1) AS MeanFireOccurrances, " + 
					 "       0 AS Frequency, " +
					 "       0 AS MFIPctDiff  " +
					 "INTO LndBpsFreq " +
					 "FROM LndBpsFo INNER JOIN LndBps " +
					 "        ON (LndBpsFo." + param->getBpsRasterName() + " = LndBps." + param->getBpsRasterName() + ") " +
					 "        AND (LndBpsFo." + param->getLndscpRasterName() + " = LndBps." + param->getLndscpRasterName() + ") " +
					 "group by LndBps.VALUE_, LndBps." + param->getLndscpRasterName() + ", " + 
					 "         LndBps." + param->getBpsRasterName() + ", LndBps.COUNT_";

		pCommand->CommandText = sQuery;
		pCommand->ExecuteNonQuery();


		// Set Mean Annual Burn and Area Burned Percentage
		// UPDATE LndBpsFreq 
		// 	 SET LndBpsFreq.MeanAnnualBurn = Round((LndBpsFreq.AreaBurned/100),2) ,
		//	 LndBpsFreq. AreaBurnedPct = (LndBpsFreq.AreaBurned/ BpsSize)*100
		sMsg = "updating LndBpsFreq - 1.";
		sQuery = "Update LndBpsFreq " +
			"set MeanAnnualBurn = Round((AreaBurned/" + Convert::ToString(param->getPeriodLen()) + "),2) ," +
			"    AreaBurnedPct = (AreaBurned/ BpsSize)*100 ";

		pCommand->CommandText = sQuery;
		pCommand->ExecuteNonQuery();


		// Set Set Mean Fire Interval and Mean Fire Occurrences 
		/*
		UPDATE LndBpsFreq 
		SET MeanFireInterval  = 
				iif(MeanAnnualBurn = 0, dmax("Frequency", "RefFreq"  ),
					iif (BpsSize/MeanAnnualBurn < dmax("Frequency", "RefFreq"  ),
						round((BpsSize/MeanAnnualBurn), 2),
						dmax("Frequency", "RefFreq"  )
					)
				) ,
			MeanFireOccurrances = Round((AreaBurned/BpsSize),2)
		*/

		sMsg = "updating LndBpsFreq - 2.";
		sQuery = 
			"UPDATE LndBpsFreq " +
			"SET MeanFireInterval  = " +
			"		iif(MeanAnnualBurn = 0, dmax(\"Frequency\", \"RefFreq\"  ), " +
			"			iif (BpsSize/MeanAnnualBurn < dmax(\"Frequency\", \"RefFreq\"  ), " +
			"				round((BpsSize/MeanAnnualBurn), 2), " +
			"				dmax(\"Frequency\", \"RefFreq\"  ) " +
			"			) " +
			"		) , " +
			"	MeanFireOccurrances = Round((AreaBurned/BpsSize),2) ";
			

		pCommand->CommandText = sQuery;
		pCommand->ExecuteNonQuery();

	
		// Set Frequency for BpS in Refcon 
		sMsg = "updating LndBpsFreq - 3.";
		sQuery = "UPDATE (LndBpsFreq " +
				 "    INNER JOIN " + param->getBpsRasterName() + " ON LndBpsFreq." + param->getBpsRasterName() + " = BpS.VALUE_) " +
				 "    INNER JOIN " + param->getRefConTableName() + " ON " + param->getBpsRasterName() + "." + param->getBpsAttribName() + " = " +
						param->getRefConTableName() + ".BpS_Model " +
				 "SET LndBpsFreq.Frequency = " +
				 "			 iif(LndBpsFreq.MeanAnnualBurn=0, " +
				 "				 iif (" + param->getRefConTableName() + ".Frequency > " + param->getPeriodLen() + ", " +
				 "                    " + param->getRefConTableName() + ".Frequency, " + param->getPeriodLen() + ") " +
				 "			 , " +
				 "				 iif ((MeanFireInterval <  " + param->getRefConTableName() + ".Frequency), " +
				 "					   MeanFireInterval " +
				 "				 , " +
				 "					   iif (MeanFireInterval >= " + param->getPeriodLen() + ", " +
				 "							iif (" + param->getRefConTableName() + ".Frequency >= " + param->getPeriodLen() + ", " + 
				 "                               " + param->getRefConTableName() + ".Frequency, " + param->getPeriodLen() + ") "
				 "                     , " +
				 "							MeanFireInterval " +
				 "					   ) " +
				 "				 ) " +
				 "			) ";

			/*
			UPDATE (HucBpsFreq 
				INNER JOIN BpS ON HucBpsFreq.BPSG = BpS.VALUE_) 
				INNER JOIN Refresh_110825 ON BpS.GROUPMODEL = Refresh_110825.BpS_Model 
			SET HucBpsFreq.Frequency = 
						 IIf(HucBpsFreq.MeanAnnualBurn=0,
							 iif (Refresh_110825.Frequency > CurrentPeriod, Refresh_110825.Frequency, CurrentPeriod     )
						 ,
							 iif ((MeanFireInterval <  Refresh_110825.Frequency),  
								   MeanFireInterval
							 ,                     
								   iif (MeanFireInterval >= CurrentPeriod,
										iif (Refresh_110825.Frequency >= CurrentPeriod, Refresh_110825.Frequency, CurrentPeriod), 
										MeanFireInterval
								   )
							 )
						);
		*/

		pCommand->CommandText = sQuery;
		pCommand->ExecuteNonQuery();


		// set Freq for BpS not in RefCon (freq still = 0) - (qUpdate_HucBpsFreq_5)
		sMsg = "updating LndBpsFreq - 4.";
		/*	UPDATE LndBpsFreq 
			SET LndBpsFreq.Frequency = 
						 IIf(MeanFireInterval > dmax("Frequency", "sSelect_RefFreq"  ) ,
							 dmax("Frequency", "sSelect_RefFreq"  ), MeanFireInterval)
			WHERE LndBpsFreq.Frequency = 0;
		*/
		sQuery = "UPDATE LndBpsFreq " +
				 "SET LndBpsFreq.Frequency = " +
				 "        IIf(MeanFireInterval > dmax(\"Frequency\", \"RefFreq\"  ) , " +
				 "		      dmax(\"Frequency\", \"RefFreq\"  ), MeanFireInterval) " +
				 "WHERE LndBpsFreq.Frequency = 0";
		pCommand->CommandText = sQuery;
		pCommand->ExecuteNonQuery(); 


		// Mean Return Interval Departure (MRID) departure and showing deficiency vs abundance
		/*
		If MFRI  < Reference Frequency 
			MRID = (1 - [MFRI / RefFreq ] ) * 100
		Else
			MRID = -1 * ((1 – [RefFreq / MFRI ] ) * 100 )  

		*/

		sMsg = "updating LndBpsFreq - 5.";
		sQuery = "UPDATE (LndBpsFreq " +
			"    INNER JOIN " + param->getBpsRasterName() + " ON LndBpsFreq." + param->getBpsRasterName() + " = BpS.VALUE_) " +
			"    INNER JOIN " + param->getRefConTableName() + " ON " + param->getBpsRasterName() + "." + param->getBpsAttribName() + " = " +
							param->getRefConTableName() + ".BpS_Model " +
			"SET LndBpsFreq.MFIPctDiff = " +
			"			 iif(LndBpsFreq.MeanFireInterval < " + param->getRefConTableName() + ".Frequency , " +
			"			     (1 - (LndBpsFreq.MeanFireInterval / " + param->getRefConTableName() + ".Frequency " +" ) ) * 100 " + 
			"            , " +
			"			     (1 - (" + param->getRefConTableName() + ".Frequency / LndBpsFreq.MeanFireInterval ) ) * -100 )";
		
		pCommand->CommandText = sQuery;
		pCommand->ExecuteNonQuery();



		return true;
	}
	catch (...)
	{
		MessageBox::Show("Error " + sMsg, 
			"Fire Frequency Tool", MessageBoxButtons::OK, MessageBoxIcon::Error);
		return false;
	}

}


bool Raster2Freq::reclassLndBps(ParamRaster2Freq ^param, String ^attribName, String ^outRasterName)
{

	IGeoProcessorResult ^gpResult;
	Geoprocessor ^gp = gcnew Geoprocessor();

	try 
	{
		// reclass LndBps on the specified column
//		arcpy.gp.Lookup_sa("LndBps","LndBpsFreq.MeanFireInterval","C:/Data/Lochsa/Outputs/freq4/mfi")
		ESRI::ArcGIS::SpatialAnalystTools::Lookup ^lkp = gcnew ESRI::ArcGIS::SpatialAnalystTools::Lookup();
		lkp->in_raster = "LndBps";
		lkp->lookup_field = "LndBpsFreq." + attribName;
		lkp->out_raster =  param->getOutputPath() + "\\" + outRasterName;

		gpResult = (IGeoProcessorResult ^)gp->Execute(lkp, nullptr);

		return true;
	}
	catch (...)
	{
		MessageBox::Show("Error reclassifying " + attribName, 
			"Fire Frequency Tool", MessageBoxButtons::OK, MessageBoxIcon::Error);
		return false;
	}

}


bool Raster2Freq::joinFreq(ParamRaster2Freq ^param)
{

	IGeoProcessorResult ^gpResult;
	Geoprocessor ^gp = gcnew Geoprocessor();

	try 
	{
		// join LndBpsFreq to the LndBps grid
		// arcpy.AddJoin_management("LndBps","VALUE","C:/Data/Lochsa/Outputs/freq1/freq.mdb/LndBpsFreq","VALUE_","KEEP_ALL")

		ESRI::ArcGIS::DataManagementTools::AddJoin ^aj = gcnew ESRI::ArcGIS::DataManagementTools::AddJoin();
		aj->in_layer_or_view = "LndBps";
		aj->in_field = "VALUE";
		aj->join_table = param->getOutputPath() + "\\freq.mdb\\LndBpsFreq"; 
		aj->join_field = "VALUE_";
		aj->join_type = "KEEP_ALL";

		gpResult = (IGeoProcessorResult ^)gp->Execute(aj, nullptr);

		return true;
	}
	catch (...)
	{
		MessageBox::Show("Error joining LndBpsFreq to LndBps.", 
			"Fire Frequency Tool", MessageBoxButtons::OK, MessageBoxIcon::Error);
		return false;
	}

}

bool Raster2Freq::createPersonalGdb(ParamRaster2Freq ^param)
{
	IGeoProcessorResult ^gpResult;
	Geoprocessor ^gp = gcnew Geoprocessor();

	try 
	{
		// create a personal geodatabase which we will use for calculating metrics

		//arcpy.CreatePersonalGDB_management("C:/Data/Lochsa/Outputs/freq4","freq","CURRENT")
		ESRI::ArcGIS::DataManagementTools::CreatePersonalGDB ^createPersGdb = gcnew ESRI::ArcGIS::DataManagementTools::CreatePersonalGDB();
		createPersGdb->out_folder_path = param->getOutputPath();
		createPersGdb->out_name = "freq.mdb"; 

		gpResult = (IGeoProcessorResult ^)gp->Execute(createPersGdb, nullptr);

//		if (!connectToPersonalGdb(param))
//			return false;
//		else
			return true;
	}
	catch (...)
	{
		MessageBox::Show("Error creating database.", 
			"Fire Frequency Tool", MessageBoxButtons::OK, MessageBoxIcon::Error);
		return false;
	}

}

bool Raster2Freq::connectToPersonalGdb(ParamRaster2Freq ^param)
{
	IGeoProcessorResult ^gpResult;
	Geoprocessor ^gp = gcnew Geoprocessor();

	try 
	{
		// Create ADO connection to the database
		String ^sFreqDbPath = param->getOutputPath() + "\\freq.mdb";
		String ^sConnectionString = "Provider=Microsoft.Jet.OLEDB.4.0; Data Source=" + sFreqDbPath;
		OleDbConnection^ pConnection = gcnew OleDbConnection(sConnectionString);
		pConnection->Open();
		param->setFreqDbConn(pConnection);

		return true;
	}
	catch (...)
	{
		MessageBox::Show("Error connecting to database.", 
			"Fire Frequency Tool", MessageBoxButtons::OK, MessageBoxIcon::Error);
		return false;
	}

}

bool Raster2Freq::exportRasterTableToGdb(ParamRaster2Freq ^param, String^ sRasterName)
{
	IGeoProcessorResult ^gpResult;
	Geoprocessor ^gp = gcnew Geoprocessor();

	try 
	{
		// export a raster attribute table to the geodatabase so we can calculate metrics

//		arcpy.TableToTable_conversion("comb_lbf","C:/Data/Lochsa/Outputs/freq6/freq.mdb","comb_lbf","#","""VALUE_ "VALUE_" false true false 4 Long 0 0 ,First,#,C:/Data/Lochsa/Outputs/freq6/comb_lbf/comb_lbf,VALUE,-1,-1;COUNT_ "COUNT_" false true false 4 Long 0 0 ,First,#,C:/Data/Lochsa/Outputs/freq6/comb_lbf/comb_lbf,COUNT,-1,-1;HUC12 "HUC12" true true false 4 Long 0 0 ,First,#,C:/Data/Lochsa/Outputs/freq6/comb_lbf/comb_lbf,HUC12,-1,-1;BPS "BPS" true true false 4 Long 0 0 ,First,#,C:/Data/Lochsa/Outputs/freq6/comb_lbf/comb_lbf,BPS,-1,-1;FO "FO" true true false 4 Long 0 0 ,First,#,C:/Data/Lochsa/Outputs/freq6/comb_lbf/comb_lbf,FO,-1,-1""","#")
//		TableToTable_conversion (in_rows, out_path, out_name, {where_clause}, {field_mapping}, {config_keyword})

		ESRI::ArcGIS::ConversionTools::TableToTable ^t2t = gcnew ESRI::ArcGIS::ConversionTools::TableToTable();
		t2t->in_rows = sRasterName;
		t2t->out_path = param->getOutputPath() + "\\freq.mdb";
		t2t->out_name = sRasterName;

		gpResult = (IGeoProcessorResult ^)gp->Execute(t2t, nullptr);

		return true;
	}
	catch (...)
	{
		MessageBox::Show("Error exporting " + sRasterName + " to metrics database.", 
			"Fire Frequency Tool", MessageBoxButtons::OK, MessageBoxIcon::Error);
		return false;
	}

}




bool Raster2Freq::combineLndscpBpsFireOcc(ParamRaster2Freq ^param)
{
	IGeoProcessorResult ^gpResult;
	Geoprocessor ^gp = gcnew Geoprocessor();

	try 
	{
		// add rasters together

		ESRI::ArcGIS::SpatialAnalystTools::Combine ^comb = gcnew ESRI::ArcGIS::SpatialAnalystTools::Combine();
		// arcpy.gp.Combine_sa("huc12;bps;fo","C:/Data/Lochsa/Outputs/freq2/lbf")
		comb->out_raster = param->getOutputPath() + "\\LndBpsFo"; 

		String ^sInRasters = param->getLndscpRasterName() + ";" +
			param->getBpsRasterName() + ";fo";
		comb->in_rasters =  sInRasters;

		gpResult = (IGeoProcessorResult ^)gp->Execute(comb, nullptr);

		return true;
	}
	catch (...)
	{
		MessageBox::Show("Error combining Landscape, BpS and Fire Occurrence rasters.", 
			"Fire Frequency Tool", MessageBoxButtons::OK, MessageBoxIcon::Error);
		return false;
	}

}

bool Raster2Freq::combineLndscpBps(ParamRaster2Freq ^param)
{
	IGeoProcessorResult ^gpResult;
	Geoprocessor ^gp = gcnew Geoprocessor();

	try 
	{
		// add rasters together

		ESRI::ArcGIS::SpatialAnalystTools::Combine ^comb = gcnew ESRI::ArcGIS::SpatialAnalystTools::Combine();
		// arcpy.gp.Combine_sa("huc12;bps","C:/Data/Lochsa/Outputs/freq2/lbf")
		comb->out_raster = param->getOutputPath() + "\\LndBps"; 

		String ^sInRasters = param->getLndscpRasterName() + ";" +
			param->getBpsRasterName();
		comb->in_rasters =  sInRasters;

		gpResult = (IGeoProcessorResult ^)gp->Execute(comb, nullptr);

		return true;
	}
	catch (...)
	{
		MessageBox::Show("Error combining Landscape and BpS rasters.", 
			"Fire Frequency Tool", MessageBoxButtons::OK, MessageBoxIcon::Error);
		return false;
	}

}


bool Raster2Freq::addRasters(ParamRaster2Freq ^param)
{
	IGeoProcessorResult ^gpResult;
	String ^ msg = "starting.";
	Geoprocessor ^gp = gcnew Geoprocessor();

	try
	{
		// add rasters together
		msg = "spatially adding fire occurrence rasters.";
		//arcpy.gp.RasterCalculator_sa(""""clw_fo" + "mtbs_fo"""","C:/Data/Lochsa/Outputs/freq/fo")
		ESRI::ArcGIS::SpatialAnalystTools::RasterCalculator ^rasCalc = gcnew ESRI::ArcGIS::SpatialAnalystTools::RasterCalculator();
		rasCalc->output_raster = param->getOutputPath() + "\\fo";
		String ^sExp = "";
		bool bFirstRaster = true;
		String ^sRasterName;
		param->resetRasterIterator();
		sRasterName = param->getNextRasterName();
		while (sRasterName != nullptr)
		{
			if (bFirstRaster)
				bFirstRaster = false;
			else
				sExp = sExp + " + ";
			sExp = sExp + "\"" + sRasterName + "\"";
			sRasterName = param->getNextRasterName();

		}
		rasCalc->expression = sExp;
		gpResult = (IGeoProcessorResult ^)gp->Execute(rasCalc, nullptr);

		return true;
	}
	catch (...)
	{
		MessageBox::Show("Error map algebraically adding rasters.  ", 
			"Fire Frequency Tool", MessageBoxButtons::OK, MessageBoxIcon::Error);

		return false;
	}

}


System::Void Raster2Freq::btnOutputFolder_Click(System::Object^  sender, System::EventArgs^  e) 
{
	this->fbdOutputFolder->Description = "Select folder where outputs will be placed.";
	if (this->fbdOutputFolder->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		this->tbOutputFolder->Text = this->fbdOutputFolder->SelectedPath;

}


void Raster2Freq::PopulateAccessTableList(System::Windows::Forms::ComboBox ^ cmb, String ^ pathName, bool firstItemToCmbText)
{
	ArrayList ^ rtl = MakeAccessTableList(pathName);

	cmb->Items->Clear();
	cmb->Items->AddRange(rtl->ToArray());
	cmb->Items->Add(L"");
	cmb->Text = L"";
	if (firstItemToCmbText)
		cmb->Text = cmb->Items[0]->ToString();
}

ArrayList ^ Raster2Freq::MakeAccessTableList(String ^ pathName)
{
	ArrayList ^ excludedTableTypeList = gcnew ArrayList(1);
	excludedTableTypeList->Add(L"MSYS");
	excludedTableTypeList->Add(L"GDB_");

	AccessUtilities ^ au = gcnew AccessUtilities();
	au->AccessTablesList(pathName, excludedTableTypeList);
	return au->TablesList;
	KillAllAccess();
}

void Raster2Freq::KillAllAccess()
{
	try
	{
		array<Process^>^ accessProcess = Process::GetProcessesByName(L"MSACCESS");
		if (accessProcess->Length > 0)
		{
			for (int i = 0; i < accessProcess->Length; i++)
			{
				accessProcess[i]->Kill();
			}
		}
	}
	catch(...)
	{
	}
}

