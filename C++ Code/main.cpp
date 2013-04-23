#include <vector>
#include <Awesomium/WebCore.h>
#include <windows.h>
#include <windowsx.h>
#include <gl/gl.h>
#include <string>
#include <iostream>
#include <mysql++.h>
#include "cmdline.h"
#include "printdata.h"
#include <iomanip>
#include <algorithm>
#include <iostream>
#include <fstream> 
#include <stdio.h>
#include <exceptions.h>
#include <time.h>
//Includes required for the application
using namespace std;; //use the standard namespace

//Defines
//#define WIDTH 1085//Application form dimension properties
//#define HEIGHT 555
#define URL	"http://moodle.ucol.ac.nz" //example of URL loading
#define GL_BGRA GL_BGRA_EXT //Define Image rendering properties


// Function Forward Declarations
//BOOL RawDataToPrinter(LPSTR szPrinterName, LPBYTE lpData, DWORD dwCount);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void EnableOpenGL(HWND hWnd, HDC * hDC, HGLRC * hRC);
void DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC);
void updateApplication(int sleepTime); //Sleep function
string ExePath(); //returns a string containing the path of the application
void mousePressed(int x, int y, int up, int type); //Send clicks to the form based on x, y co-ordinates
void newProd(string prodID, string prodName, string prodDesc, string prodCat, double prodPrice, UINT prodStock);
// New Product using the product class
void mouseMoved(int x, int y);//sends mouse co-ordinates to the form
int delta; //delta co-ords for mouse wheel
UINT Id;
POINT mouse; // set the mouse up as a POINT
void getProds(); 
void getProd(string iD);
bool active = false; //check if a certain function is still active or not
//Conversion methods
std::string intToString(int i);
std::string doubleToString(double i);
int wstringToint(std::wstring b);
string wstringToString(std::wstring b);
double wstringToDouble(std::wstring b);
UINT wstringToUint(std::wstring b);
//update method
void updateStock(int ID, int Stock);
string openfilename();
void newOrder(std::string date, double Price, int prodID, std::string time, int orderID, int qty, double eftpos, double cash);
int getLastID();
void getCategory();
void purchaseList(string date);
void deleteProduct(string prodId);
int tWidth = 1080;
int tHeight = 560;
BOOL isMousePressed = FALSE;
BOOL isMaxd = FALSE;
int cnt = 0;
int cou = 0;
const char *db = "thebusiness", *server = "127.0.0.1:3306", *user = "root", *pass = "root"; //Pointer to a char that has the database details in it
//Order Item Class
class orderItem
{
	string Order_date,Order_time,Product_name,Product_category, Product_id;
	double Order_price,Eftpos,Cash;
	int Order_id,Qty;
public:
	string Order_Date(){return Order_date;}
	string Order_Time(){return Order_time;}
	string Order_Product_Name(){return Product_name;}
	string Order_Product_Category(){return Product_category;}
	double Order_Price(){return Order_price;}
	double Order_Eftpos(){return Eftpos;}
	double Order_Cash(){return Cash;}
	int Order_ID(){return Order_id;}
	string Order_Product_ID(){return Product_id;}
	int Order_qty(){return Qty;}
	orderItem(string order_date, string order_time,string order_product_name,string order_product_category,double order_price, double order_eftpos, double order_cash, int order_id, string product_id, int qty);
};
orderItem::orderItem(std::string order_date, std::string order_time, std::string order_product_name, std::string order_product_category, double order_price, double order_eftpos, double order_cash, int order_id, string product_id, int qty)
{
	Order_date = order_date;
	Order_time = order_time;
	Product_name = order_product_name;
	Product_category = order_product_category;
	Order_price = order_price;
	Eftpos = order_eftpos;
	Cash = order_cash;
	Order_id = order_id;
	Product_id = product_id;
	Qty = qty;
};
//Product Class
class product
{
	UINT ID;
	string Product_ID,Product_Name,Product_Description,Product_Category;
	double Product_Price;
	UINT Product_Stock;
	UINT Qty;
public:
	UINT Id(){return ID;}
	string Product_Id(){return Product_ID;}
	string Product_name(){return Product_Name;}
	string Product_desc(){return Product_Description;}
	string Product_category(){return Product_Category;}
	double Product_price(){return Product_Price;}
	UINT Product_stock(){return Product_Stock;}
	UINT QTY(){return Qty;}
	void SetQTY(UINT qty){Qty = qty;}
	product(UINT id, string Pid, string Name, string Desc, string Category, double Price, UINT Stock );
	product(UINT id, string Pid, string Name, string Desc, string Category, double Price, UINT Stock, UINT qty );

};
product::product(UINT id, std::string Pid, std::string Name,std::string Desc, std::string Category, double Price, UINT Stock)
{
	ID = id;
	Product_ID = Pid;
	Product_Name = Name;
	Product_Description = Desc;
	Product_Category = Category;
	Product_Price = Price;
	Product_Stock = Stock;
}
product::product(UINT id, std::string Pid, std::string Name,std::string Desc, std::string Category, double Price, UINT Stock, UINT qty)
{
	ID = id;
	Product_ID = Pid;
	Product_Name = Name;
	Product_Description = Desc;
	Product_Category = Category;
	Product_Price = Price;
	Product_Stock = Stock;
	Qty = qty;
}
//vectors to hold Order details etc.
vector<orderItem> OrderItems;
vector<product> SS;
vector<string> CatNames;
vector<int> CatAlts;
//Create Awesomium class so I can access the correct methods / properties
class CAwesomium : public Awesomium::WebViewListener
{
public:
	CAwesomium::CAwesomium();
	Awesomium::WebView* myWebView;
	Awesomium::WebCore* myWebCore;
	Awesomium::WebCoreConfig config;
	virtual void 	onFinishLoading (Awesomium::WebView *caller);
	virtual void 	onCallback (Awesomium::WebView *caller, const std::wstring &objectName, const std::wstring &callbackName, const Awesomium::JSArguments &args);

	virtual void 	onBeginNavigation (Awesomium::WebView *caller, const std::string &url, const std::wstring &frameName){}
	virtual void 	onBeginLoading (Awesomium::WebView *caller, const std::string &url, const std::wstring &frameName, int statusCode, const std::wstring &mimeType){}
	virtual void 	onReceiveTitle (Awesomium::WebView *caller, const std::wstring &title, const std::wstring &frameName){}
	virtual void 	onChangeTooltip (Awesomium::WebView *caller, const std::wstring &tooltip){}
	virtual void 	onChangeCursor (Awesomium::WebView *caller, Awesomium::CursorType cursor){}
	virtual void 	onChangeKeyboardFocus (Awesomium::WebView *caller, bool isFocused){}
	virtual void 	onChangeTargetURL (Awesomium::WebView *caller, const std::string &url){}
	virtual void 	onOpenExternalLink (Awesomium::WebView *caller, const std::string &url, const std::wstring &source){}
	virtual void 	onRequestDownload (Awesomium::WebView *caller, const std::string &url){}
	virtual void 	onWebViewCrashed (Awesomium::WebView *caller){}
	virtual void 	onPluginCrashed (Awesomium::WebView *caller, const std::wstring &pluginName){}
	virtual void 	onRequestMove (Awesomium::WebView *caller, int x, int y){}
	virtual void 	onGetPageContents (Awesomium::WebView *caller, const std::string &url, const std::wstring &contents){}
	virtual void 	onDOMReady (Awesomium::WebView *caller){}
	virtual void 	onRequestFileChooser (Awesomium::WebView *caller, bool selectMultipleFiles, const std::wstring &title, const std::wstring &defaultPath){}
	virtual void 	onGetScrollData (Awesomium::WebView *caller, int contentWidth, int contentHeight, int preferredWidth, int scrollX, int scrollY){}
	virtual void 	onJavascriptConsoleMessage (Awesomium::WebView *caller, const std::wstring &message, int lineNumber, const std::wstring &source){}
	virtual void 	onGetFindResults (Awesomium::WebView *caller, int requestID, int numMatches, const Awesomium::Rect &selection, int curMatch, bool finalUpdate){}
	virtual void 	onUpdateIME (Awesomium::WebView *caller, Awesomium::IMEState imeState, const Awesomium::Rect &caretRect){}
	virtual void 	onShowContextMenu (Awesomium::WebView *caller, int mouseX, int mouseY, Awesomium::MediaType type, int mediaState, const std::string &linkURL, const std::string &srcURL, const std::string &pageURL, const std::string &frameURL, const std::wstring &selectionText, bool isEditable, int editFlags){}
	virtual void 	onRequestLogin (Awesomium::WebView *caller, int requestID, const std::string &requestURL, bool isProxy, const std::wstring &hostAndPort, const std::wstring &scheme, const std::wstring &realm){}
	virtual void 	onChangeHistory (Awesomium::WebView *caller, int backCount, int forwardCount){}
	virtual void 	onFinishResize (Awesomium::WebView *caller, int width, int height){}
	virtual void 	onShowJavascriptDialog (Awesomium::WebView *caller, int requestID, int dialogFlags, const std::wstring &message, const std::wstring &defaultPrompt, const std::string &frameURL){}
};
class ESCPOSDevice
{
private:
	ofstream out;

	// Codes
	//static const char* InitalizePrinter="\x1B\x40";
	//static const char* CutPaper="\x1D\x56\x42\x00\x1B\x70\x00\x40\xF0";
	//"\x1B\x70\x00\x40\xF0"
	//static const char[] OpenDraw={ 0x1B, 0x70, 0x00, 0x40, 0xF0 };


	//ESC/POS constants standard commanding to receipt printer
	static const char LF = 0x0A;
	static const char ESC = 0x1B;
	static const char GS = 0x1D;

	//ESC/POS constants standard commanding to customer display
	static const char NLL = 0x00;
	static const char MD1 = 0x01;
	static const char MD2 = 0x02;
	static const char MD3 = 0x03;
	static const char MD4 = 0x04;
	static const char MD5 = 0x05;
	static const char MD6 = 0x06;
	static const char MD7 = 0x07;
	static const char MD8 = 0x08;
	static const char BS = 0x08;
	static const char HT = 0x09;
	//static const char LF = 0xA;
	static const char HOM = 0x0B;
	static const char CLR = 0x0C;
	static const char CR = 0x0D;
	static const char SLE1 = 0x0E;
	static const char rs = 0x0F;
	static const char SLE2 = 0x0F;
	static const char DLE = 0x10;
	static const char DC1 = 0x11;
	static const char DC2 = 0x12;
	static const char DC3 = 0x13;
	static const char DC4 = 0x14;
	static const char CAN = 0x18;
	//static const char ESC = 0x1B;
	static const char SF1 = 0x1E;
	static const char US = 0x1F, SF2 = 0x1F;
	static const char BEL = 0x07;

public:
	char Port[255];
	ESCPOSDevice()
	{
		Port[50]=NULL;
		// Set out to default prehaps
		strcpy(Port,"//thebusiness-SERVER/receipt");
	}

	ESCPOSDevice(const char* portName)
	{
		changePort(portName);
	}

	~ESCPOSDevice()
	{
		if (out.is_open())
		{
			out.close();
		}			
	}

	void changePort(const char* portName)
	{
		if (out.is_open())
		{
			out.close();
		}
		strcpy(Port,portName);
	}

	void open()
	{
		out.open(Port,ios::out | ios::binary);
	}

	bool isOpen()
	{
		return out.is_open();
	}
	// deviceNumber
	// must be ascii number not a decimal value of 1 or 2
	// "1" == printer
	// "2" == display
	void selectDevice(char deviceNumber)
	{
		out << ESC << "=" << deviceNumber;
	}


	void soundBell()
	{
		out << ESC << BEL;
	}

	void disablePaperOutDetector()
	{
		out << ESC << MD8;
	}

	void enablePaperOutdetector()
	{
		out << ESC << HT;
	}

	// 0,48-pin 2    1,49-pin 5
	void generatePulse(const char pin)
	{
		out << ESC << "p" << pin << (char)0x50 << (char)0x50;
	}

	void openCashDrawer()
	{
		//0x40, 0xF0
		out << ESC << "p" << (char)0x00 << (char)0x32 << (char)0xFA;
	}

	void boldON()
	{
		out << ESC << "E" << (char)0x01;
	}

	void boldOFF()
	{
		out << ESC << "E" << (char)0x00;
	}

	void printDoubleAndFeed(const char* line, double price)
	{
		out << line << fixed << setprecision(2) << price;
		out << LF;
	}

	void printAndFeed(const char* line)
	{
		out << line;
		out << LF;
	}

	void setJust(int j)
	{
		switch (j)
		{
			//left
		case 1:
			out << ESC << "a" << (char)0x00;
			break;
			//Center
		case 2:
			out << ESC << "a" << (char)0x01;
			break;
			//Right
		case 3:
			out << ESC << "a" << (char)0x02;
			break;
		}
	}

	void cutPaper()
	{
		out << LF;
		out << LF;
		out << GS << "V" << (char)0x01;
	}

	void cutPaperFull()
	{
		out << ESC << "GSV66";
	}

	void output(const char* message)
	{
		out << message;
	}

	void initPrinter()
	{
		out << ESC << "@";
	}

	ofstream& getOutput()
	{
		return out;
	}

	void flush()
	{
		out.flush();
	}

	void close()
	{
		out.close();
	}

};
// Implement the Awesomium class so I can access its properties globally
CAwesomium aws;
CAwesomium::CAwesomium()
{
	aws.config.setEnableJavascript(true); //Enable Javascript so we can access callbacks
	aws.myWebCore = new Awesomium::WebCore(aws.config); //Create a new web core so we can send HTML etc to awesomium
	aws.myWebView = aws.myWebCore->createWebView(tWidth, tHeight); //Create a new web view so we can view the HTML etc
	aws.myWebCore->setBaseDirectory(ExePath()); //Set the base directory
	aws.myWebView->setListener(this); //We can now listen for Javascript callbacks
	aws.myWebView->loadFile("index.html"); //Load an HTML file
	aws.myWebView->focus(); //Gain focus

}
void CAwesomium::onFinishLoading(Awesomium::WebView *caller) //Stuff to do once the HTML page has been loaded
{
	//Specify what Javascript callbacks we want to listen for
	aws.myWebView->createObject(L"ProductList");
	aws.myWebView->setObjectCallback(L"ProductList", L"List");
	aws.myWebView->createObject(L"Products");
	aws.myWebView->setObjectCallback(L"Products", L"Load");
	aws.myWebView->createObject(L"ProductClear");
	aws.myWebView->setObjectCallback(L"ProductClear", L"Clear");
	aws.myWebView->createObject(L"ProductRemove");
	aws.myWebView->setObjectCallback(L"ProductRemove", L"Remove");
	aws.myWebView->createObject(L"newProduct");
	aws.myWebView->setObjectCallback(L"newProduct", L"Create");
	aws.myWebView->createObject(L"Stock");
	aws.myWebView->setObjectCallback(L"Stock", L"Update");
	aws.myWebView->createObject(L"Search");
	aws.myWebView->setObjectCallback(L"Search", L"Execute");
	aws.myWebView->createObject(L"Orders");
	aws.myWebView->setObjectCallback(L"Orders", L"newOrder");
	aws.myWebView->setObjectCallback(L"Orders", L"getLastID");
	aws.myWebView->setObjectCallback(L"Orders", L"List");
	aws.myWebView->setObjectCallback(L"Orders", L"Print");
	aws.myWebView->createObject(L"Picture");
	aws.myWebView->setObjectCallback(L"Picture", L"Get");
	aws.myWebView->setObjectCallback(L"Picture", L"FromDisk");
	aws.myWebView->createObject(L"Category");
	aws.myWebView->setObjectCallback(L"Category", L"Get");
	aws.myWebView->createObject(L"Product");
	aws.myWebView->setObjectCallback(L"Product", L"Delete");
}
//When a javascript callback is fired
void CAwesomium::onCallback(Awesomium::WebView* caller, 
							const std::wstring& objectName, 
							const std::wstring& callbackName, 
							const Awesomium::JSArguments& args)
{
	//Specify which callback were working with
	if(objectName == L"ProductList" && callbackName == L"List" && active == false)
	{
		try{
			active = true;
			getProds(); //Get our products
			int size; //an integer to store the vector size
			size= SS.size(); //get the vector size
			for (int i=0;i<size;i++)
			{
				Awesomium::JSArguments args; //The arguements assigned to the Javascript callback
				args.clear();
				args.push_back(intToString(SS[i].Id())); //Send an arguement
				args.push_back(SS[i].Product_Id());
				args.push_back(SS[i].Product_name());
				args.push_back(SS[i].Product_desc());
				args.push_back(SS[i].Product_category());
				args.push_back(SS[i].Product_price());
				args.push_back(intToString(SS[i].Product_stock()));
				args.push_back(0);
				string imgstr = SS[i].Product_Id()+".jpg"; //The image name we are going to load
				string str = ExePath() +"\\" + imgstr; //Where the image is going to be found
				char* filename = new char[str.size() + 1]; //A new character pointer that will hold the filename
				std::copy(str.begin(), str.end(), filename); //conversion time
				filename[str.size()] = '\0'; //trailing end of file to prevent errors
				ifstream pic(filename); //filestream
				if (pic.good()) //if it exists
				{
					args.push_back(imgstr); //send the file location to the application
				}
				else
				{
					string noPic = "Default.jpg"; //we either have no image for the product or the name is wrong
					args.push_back(noPic);
				}
				aws.myWebView->callJavascriptFunction(L"", L"loadProduct", args); //Call the loadProduct Javascript function
			}
			Awesomium::JSArguments args;
			args.clear();
			aws.myWebView->callJavascriptFunction(L"", L"listbox", args);
			SS.clear();
			active = false;
		}
		catch (const mysqlpp::ConnectionFailed &ex) //Catch a mysql error that is thrown when the database isnt running
		{
			//create a new messagebox with an error message stating that the database isnt running
			MessageBox(NULL, "Connection to the database failed,\n Make sure it is running and restart the application.", "Error", MB_OK);
		}
		catch(...) //Catch any error
		{
			//Send generic error message
			MessageBox(NULL, "There was an error, try restarting application.", "Error", MB_OK);
		}
	}
	if(objectName == L"Products" && callbackName == L"Load" && active == false)
	{
		try{
			active = true;

			std::wstring pId = args[0].toString();
			string prodId = wstringToString(pId);
			getProd(prodId);
			int size;
			size= SS.size();
			double totalPrice =0;
			for (int i=0;i<size;i++)
			{
				totalPrice = totalPrice + (SS[i].Product_price() * SS[i].QTY()); //Some basic maths to find the total price
				Awesomium::JSArguments args;
				args.clear();
				args.push_back(intToString(SS[i].Id()));
				args.push_back(SS[i].Product_Id());
				args.push_back(SS[i].Product_name());
				args.push_back(SS[i].Product_desc());
				args.push_back(SS[i].Product_category());
				args.push_back(SS[i].Product_price());
				args.push_back(intToString(SS[i].Product_stock()));
				args.push_back(intToString(SS[i].QTY()));
				string imgstr = SS[i].Product_Id()+".jpg";
				string str = ExePath() +"\\" + imgstr;
				char* filename = new char[str.size() + 1];
				std::copy(str.begin(), str.end(), filename);
				filename[str.size()] = '\0';
				ifstream pic(filename);
				if (pic.good())
				{
					args.push_back(imgstr);
				}
				else
				{
					string noPic = "Default.jpg";
					args.push_back(noPic);
				}
				aws.myWebView->callJavascriptFunction(L"", L"loadProduct", args);
			}
			Awesomium::JSArguments args;
			args.clear();
			args.push_back(totalPrice);
			aws.myWebView->callJavascriptFunction(L"", L"listbox", args);
			active = false;
		}
		catch (const mysqlpp::ConnectionFailed &ex) 
		{
			MessageBox(NULL, "Connection to the database failed, \n make sure it is running and restart the application", "Error", MB_OK);
		}
		catch(...)
		{
			MessageBox(NULL, "There was an error, try restarting application.", "Error", MB_OK);
		}
	}
	if(objectName == L"ProductClear" && callbackName == L"Clear" && active == false)
	{
		SS.clear(); //Clear the SS vector
	}
	if(objectName == L"ProductRemove" && callbackName == L"Remove" && active == false) //used to remove products from the purchase list
	{
		try
		{
			active = true;
			int prodId = args[0].toInteger();
			int size;
			size= SS.size();
			double totalPrice =0;
			vector<product> temp;
			for (int i=0;i<size;i++)
			{
				if (SS[i].Id() == prodId)
				{
					SS[i].SetQTY(SS[i].QTY() - 1); //decrease the amount of the product in the purchase
				}
				if (SS[i].QTY() == 0) //if the amount is 0
				{
					//Update the vector to reflect the change
					for (int j=0;j<size;j++)
					{
						if (SS[j].QTY() != 0)
						{
							temp.push_back(SS[j]);
						}
					}
					SS.clear();
					SS.assign(temp.begin(), temp.end());
					size= SS.size();
				}

			}
			for (int i=0;i<size;i++)
			{
				//show the changes in the application
				totalPrice = totalPrice + (SS[i].Product_price() * SS[i].QTY());
				Awesomium::JSArguments args;
				args.clear();
				args.push_back(intToString(SS[i].Id()));
				args.push_back(SS[i].Product_Id());
				args.push_back(SS[i].Product_name());
				args.push_back(SS[i].Product_desc());
				args.push_back(SS[i].Product_category());
				args.push_back(SS[i].Product_price());
				args.push_back(intToString(SS[i].Product_stock()));
				args.push_back(intToString(SS[i].QTY()));
				aws.myWebView->callJavascriptFunction(L"", L"loadProduct", args);
			}
			//reload the purchase
			Awesomium::JSArguments args;
			args.clear();
			args.push_back(totalPrice);
			aws.myWebView->callJavascriptFunction(L"", L"reloadPurchase", args);
			active = false;
		}
		catch (const mysqlpp::ConnectionFailed &ex) 
		{
			MessageBox(NULL, "Connection to the database failed, \n make sure it is running and restart the application", "Error", MB_OK);
		}
		catch(...)
		{
			MessageBox(NULL, "There was an error, try restarting application.", "Error", MB_OK);
		}
	}
	if(objectName == L"newProduct" && callbackName == L"Create" && active == false) //create a new product
	{
		try
		{
			active = true;
			string tempID = wstringToString(args[0].toString());
			string tempName = wstringToString(args[1].toString());
			string tempDesc = wstringToString(args[2].toString());
			string tempCat = wstringToString(args[3].toString());
			double tempPrice = wstringToDouble(args[4].toString());
			UINT tempStock = wstringToint(args[5].toString());

			newProd(tempID,tempName,tempDesc,tempCat,tempPrice,tempStock);
			active = false;
		}
		catch (const mysqlpp::ConnectionFailed &ex) 
		{
			MessageBox(NULL, "Connection to the database failed, \n make sure it is running and restart the application", "Error", MB_OK);
		}
		catch(...)
		{
			MessageBox(NULL, "There was an error, try restarting application.", "Error", MB_OK);
		}
	}
	if(objectName == L"Stock" && callbackName == L"Update" && active == false) //used to update a products stock and to reflect the changes to the application
	{
		try
		{
			active = true;
			int updateID = args[0].toInteger();
			int newStock = args[1].toInteger();

			updateStock(updateID,newStock);
			active = false;
		}
		catch (const mysqlpp::ConnectionFailed &ex) 
		{
			MessageBox(NULL, "Connection to the database failed, \n make sure it is running and restart the application", "Error", MB_OK);
		}
		catch(...)
		{
			MessageBox(NULL, "There was an error, try restarting application.", "Error", MB_OK);
		}
	}
	if(objectName == L"Search" && callbackName == L"Execute" && active == false) //Search products and update the listbox based on the returned products
	{
		try
		{
			SS.clear();
			getProds();
			active = true;
			std::wstring argQ = args[0].toString();
			string searchString = wstringToString(argQ);
			int size;
			size= SS.size();
			vector<product>temp;
			for (int i=0;i<size;i++)
			{
				//Make the search statement case insensitive
				string pn = SS[i].Product_name();
				transform(searchString.begin(), searchString.end(),searchString.begin(), ::toupper);
				transform(pn.begin(), pn.end(),pn.begin(), ::toupper);
				if (std::string::npos != pn.find(searchString))
				{
					temp.push_back(SS[i]);
				}
			}
			SS.clear();
			size = temp.size();
			for (int k=0;k<size;k++)
			{
				SS.push_back(temp[k]);
			}
			size= SS.size();
			for (int j=0;j<size;j++)
			{
				Awesomium::JSArguments args;
				args.clear();
				args.push_back(intToString(SS[j].Id()));
				args.push_back(SS[j].Product_Id());
				args.push_back(SS[j].Product_name());
				args.push_back(SS[j].Product_desc());
				args.push_back(SS[j].Product_category());
				args.push_back(SS[j].Product_price());
				args.push_back(intToString(SS[j].Product_stock()));
				args.push_back(0);
				string imgstr = SS[j].Product_Id()+".jpg";
				string str = ExePath() +"\\" + imgstr;
				char* filename = new char[str.size() + 1];
				std::copy(str.begin(), str.end(), filename);
				filename[str.size()] = '\0';
				ifstream pic(filename);
				if (pic.good())
				{
					args.push_back(imgstr);
				}
				else
				{
					string noPic = "Default.jpg";
					args.push_back(noPic);
				}
				aws.myWebView->callJavascriptFunction(L"", L"loadProduct", args);
			}
			Awesomium::JSArguments args;
			args.clear();
			aws.myWebView->callJavascriptFunction(L"", L"listbox", args);
			SS.clear();
			active = false;
		}
		catch (const mysqlpp::ConnectionFailed &ex) 
		{
			MessageBox(NULL, "Connection to the database failed, \n make sure it is running and restart the application", "Error", MB_OK);
		}
		catch(...)
		{
			MessageBox(NULL, "There was an error, try restarting application.", "Error", MB_OK);
		}
	}
	if(objectName == L"Orders" && callbackName == L"Print" && active == false)
	{
		try
		{
			active = true;
			if (MessageBox(NULL, "Would you like to print a receipt?", "Confirmation", MB_YESNOCANCEL)==IDYES)
			{
				BOOL openDrawer = FALSE;
				int lastID = getLastID();
				OrderItems.clear();
				int totalPrice = 0;
				mysqlpp::Connection conn;
				if (conn.connect(db, server,
					user, pass)) 
				{
					mysqlpp::Query qq = conn.query("SELECT * FROM `thebusiness`.`orders` INNER JOIN `thebusiness`.`products` ON `orders`.`Prod_ID`=`products`.`ID` INNER JOIN `thebusiness`.`payment` ON `orders`.`Order_ID`=`payment`.`Order_ID` WHERE `thebusiness`.`orders`.`Order_ID` = '"+intToString(lastID)+"' AND (`thebusiness`.`payment`.`Eftpos` + `thebusiness`.`payment`.`Cash`) >'0' ORDER BY `orders`.`Order_ID` DESC;");
					if (mysqlpp::StoreQueryResult res = qq.store()) 
					{
						int as = res.num_rows();
						for (size_t i = 0; i < res.num_rows(); i++) 
						{
							string tempDate = res[i]["Order_Date"];
							string tempTime = res[i]["Order_Time"];
							string tempName = res[i]["Product_Name"];
							string tempCat = res[i]["Product_Category"];
							double tempPrice = res[i]["Product_Price"];
							double tempEftpos = res[i]["Eftpos"];
							double tempCash = res[i]["Cash"];
							int tempOrderId = res[i]["Order_ID"];
							string tempProdId = res[i]["Product_ID"];
							int tempQty = res[i]["Qty"];
							totalPrice = totalPrice + (tempPrice * tempQty);
							orderItem t (tempDate,tempTime,tempName,tempCat,tempPrice,tempEftpos,tempCash,tempOrderId,tempProdId,tempQty);
							OrderItems.push_back(t);
						}

					}

					ESCPOSDevice* cashRegister = new ESCPOSDevice();
					cashRegister->open();
					if (cashRegister->isOpen())
					{

						//printf("Success\n");
						//cashRegister->soundBell();

						cashRegister->initPrinter();
						time_t rawtime;
						struct tm * timeinfo;
						time ( &rawtime );
						timeinfo = localtime ( &rawtime );
						cashRegister->setJust(2);
						cashRegister->printAndFeed("");
						cashRegister->printAndFeed(asctime (timeinfo));
						cashRegister->printAndFeed("The thebusiness, 86 Broadway Avenue");
						cashRegister->printAndFeed("Palmerston North");
						cashRegister->setJust(1);
						cashRegister->printAndFeed("");
						cashRegister->printAndFeed("");
						int size = OrderItems.size();
						for (int i=0;i<size;i++)
						{
							std::string b;
							cashRegister->printAndFeed("");
							if(OrderItems[i].Order_qty()>1)
							{
								b = OrderItems[i].Order_Product_Name() + " x"+ intToString(OrderItems[i].Order_qty()) +"  $";
								char a [255] = {0};
								std::copy(b.begin(), b.end(), a);
								double it = OrderItems[i].Order_Price() * OrderItems[i].Order_qty();
								cashRegister->printDoubleAndFeed(a, it);
							}
							else
							{
								b = OrderItems[i].Order_Product_Name() + "  $";
								char a [255] = {0};
								std::copy(b.begin(), b.end(), a);
								double it = OrderItems[i].Order_Price();
								cashRegister->printDoubleAndFeed(a, it);
							}
							char a [255] = {0};
						}
						cashRegister->printAndFeed("");
						cashRegister->printAndFeed("");
						cashRegister->boldON();
						std::string b = "Total Price: $";
						char a [23] = {0};
						std::copy(b.begin(), b.end(), a);
						double total = totalPrice;
						cashRegister->printDoubleAndFeed(a, total);
						cashRegister->boldOFF();
						if (OrderItems[0].Order_Cash() > 0)
						{
							std::fill(&a[0], &a[sizeof(a)], 0);
							b = "Cash Paid: $";
							std::copy(b.begin(), b.end(), a);
							cashRegister->printDoubleAndFeed(a, OrderItems[0].Order_Cash());
							openDrawer = TRUE;
						}
						if (OrderItems[0].Order_Eftpos() > 0)
						{
							std::fill(&a[0], &a[sizeof(a)], 0);
							b = "Eftpos Paid: $";
							std::copy(b.begin(), b.end(), a);
							cashRegister->printDoubleAndFeed(a, OrderItems[0].Order_Eftpos());
						}
						std::fill(&a[0], &a[sizeof(a)], 0);
						b = "Total Payment: $";
						double tot = OrderItems[0].Order_Cash() + OrderItems[0].Order_Eftpos();
						std::copy(b.begin(), b.end(), a);
						cashRegister->printDoubleAndFeed(a, tot);
						cashRegister->printAndFeed("");
						if ((OrderItems[0].Order_Eftpos() + OrderItems[0].Order_Cash()) - totalPrice > 0 )
						{
							std::fill(&a[0], &a[sizeof(a)], 0);
							double change = (OrderItems[0].Order_Eftpos() + OrderItems[0].Order_Cash()) - totalPrice;
							b = "Change: $";
							std::copy(b.begin(), b.end(), a);
							cashRegister->printDoubleAndFeed(a, change);
							openDrawer = TRUE;
						}
						cashRegister->printAndFeed("");
						cashRegister->printAndFeed("");
						cashRegister->cutPaper();
						if (openDrawer == TRUE)
						{
							cashRegister->openCashDrawer();
						}
						//cashRegister->cutPaperFull();
						//cashRegister->flush();
						cashRegister->close();
					}
					else
					{
						//printf("Fail\n");
					}
				}
			}

			active = false;
		}
		catch (const mysqlpp::ConnectionFailed &ex) 
		{
			MessageBox(NULL, "Connection to the database failed, \n make sure it is running and restart the application", "Error", MB_OK);
		}
		catch(...)
		{
			MessageBox(NULL, "There was an error, try restarting application.", "Error", MB_OK);
		}

	}
	if(objectName == L"Orders" && callbackName == L"newOrder" && active == false) //Process the order through the system
	{
		try
		{
			active = true;
			std::string orderDate = wstringToString(args[0].toString());
			double orderPrice = args[1].toDouble();
			INT productID = args[2].toInteger();
			std::string orderTime = wstringToString(args[3].toString());
			int orderID = args[4].toInteger();
			int qty = args[5].toInteger();
			double eftpos = args[6].toDouble();
			double cash = args[7].toDouble();
			newOrder(orderDate,orderPrice,productID,orderTime,orderID,qty,eftpos,cash);
			active = false;
		}
		catch (const mysqlpp::ConnectionFailed &ex) 
		{
			MessageBox(NULL, "Connection to the database failed, \n make sure it is running and restart the application", "Error", MB_OK);
		}
		catch(...)
		{
			MessageBox(NULL, "There was an error, try restarting application.", "Error", MB_OK);
		}
	}
	if(objectName == L"Orders" && callbackName == L"getLastID" && active == false)
	{
		try
		{
			active = true;
			int lastID = getLastID();
			lastID++;
			Awesomium::JSArguments args;
			args.clear();
			args.push_back(lastID);
			aws.myWebView->callJavascriptFunction(L"", L"setLastID", args);
			active = false;
		}
		catch (const mysqlpp::ConnectionFailed &ex) 
		{
			MessageBox(NULL, "Connection to the database failed, \n make sure it is running and restart the application", "Error", MB_OK);
		}
		catch(...)
		{
			MessageBox(NULL, "There was an error, try restarting application.", "Error", MB_OK);
		}
	}
	if(objectName == L"Picture" && callbackName == L"Get" && active == false)// Gets and saves the image we click on in the application
	{
		try
		{
			active = true;
			Awesomium::WebView* temp; //create a new non visible "Tab" in the application
			temp = myWebCore->createWebView(args[0].toInteger(), args[1].toInteger() + 4);
			temp->loadURL(args[2].toString());
			while(temp->isLoadingPage())
			{
				updateApplication(50);//update the application
			}
			if (!temp->isLoadingPage())
			{
				updateApplication(500); //Sleep for 500ms
				int a = args[0].toInteger();
				int b = args[1].toInteger();
				std::wstring page = args[2].toString();
				std::wstring name = args[3].toString();
				temp->resize(args[0].toInteger(),args[1].toInteger()); //resize the Tab to the dimensions of the picture
				temp->render()->saveToJPEG(args[3].toString(),100); //Take a screenshot with 100% quality and save it to Disk
			}
			temp->destroy(); //Destroy the Tab
			active = false;
		}
		catch (const mysqlpp::ConnectionFailed &ex) 
		{
			MessageBox(NULL, "Connection to the database failed, \n make sure it is running and restart the application", "Error", MB_OK);
		}
		catch(...)
		{
			MessageBox(NULL, "There was an error, try restarting application.", "Error", MB_OK);
		}
	}
	if (objectName == L"Picture" && callbackName == L"FromDisk" && active == false) //Load a picture for a new product from the users drive instead of getting one from google
	{
		try
		{
			active = true;
			string srcStr = openfilename(); //Create a new openfilename dialog and get the name of the file the user has selected
			//Conversion time
			char* srcFilename = new char[srcStr.size() + 1];
			std::copy(srcStr.begin(), srcStr.end(), srcFilename);
			srcFilename[srcStr.size()] = '\0';	
			std::ifstream  src(srcFilename, std::ios::binary);

			std::wstring sstr = args[0].toString();
			string str = ExePath() + "\\" + wstringToString(sstr); 
			char* filename = new char[str.size() + 1];
			std::copy(str.begin(), str.end(), filename);
			filename[str.size()] = '\0';
			std::ofstream  dst(filename, std::ios::binary);

			dst << src.rdbuf();
			Awesomium::JSArguments args;
			args.clear();
			args.push_back(sstr);
			aws.myWebView->callJavascriptFunction(L"", L"loadImage", args);
			active = false;
		}
		catch (const mysqlpp::ConnectionFailed &ex) 
		{
			MessageBox(NULL, "Connection to the database failed, \n make sure it is running and restart the application", "Error", MB_OK);
		}
		catch(...)
		{
			MessageBox(NULL, "There was an error, try restarting application.", "Error", MB_OK);
		}
	}
	if (objectName == L"Category" && callbackName == L"Get" && active == false) //get the product categories for the application
	{
		try
		{
			active = true;
			CatNames.clear();
			CatAlts.clear();
			getCategory();
			Awesomium::JSArguments args;
			args.clear();
			int size = CatNames.size();
			for(int i=0;i<size;i++)
			{
				args.clear();
				args.push_back(CatNames[i]);
				args.push_back(CatAlts[i]);
				aws.myWebView->callJavascriptFunction(L"", L"loadCategory", args);
			}
			args.clear();
			aws.myWebView->callJavascriptFunction(L"", L"newPrduct", args);
			active = false;
		}
		catch (const mysqlpp::ConnectionFailed &ex) 
		{
			MessageBox(NULL, "Connection to the database failed, \n make sure it is running and restart the application", "Error", MB_OK);
		}
		catch(...)
		{
			MessageBox(NULL, "There was an error, try restarting application.", "Error", MB_OK);
		}
	}
	if (objectName == L"Orders" && callbackName == L"List" && active == false)// List the orders to the application
	{
		try
		{
			active = true;
			OrderItems.clear();
			string tempdate = wstringToString(args[0].toString());
			purchaseList(tempdate);
			Awesomium::JSArguments args;
			int size = OrderItems.size();
			for(int i=0;i<size;i++)
			{
				args.clear();
				args.push_back(OrderItems[i].Order_Date());
				args.push_back(OrderItems[i].Order_Time());
				args.push_back(OrderItems[i].Order_Product_Name());
				args.push_back(OrderItems[i].Order_Product_Category());
				args.push_back(OrderItems[i].Order_Price());
				args.push_back(OrderItems[i].Order_Eftpos());
				args.push_back(OrderItems[i].Order_Cash());
				args.push_back(OrderItems[i].Order_ID());
				args.push_back(OrderItems[i].Order_Product_ID());
				args.push_back(OrderItems[i].Order_qty());
				aws.myWebView->callJavascriptFunction(L"", L"loadOrder", args);
			}
			args.clear();
			aws.myWebView->callJavascriptFunction(L"", L"listOrder", args);
			active = false;
		}
		catch (const mysqlpp::ConnectionFailed &ex) 
		{
			MessageBox(NULL, "Connection to the database failed, \n make sure it is running and restart the application", "Error", MB_OK);
		}
		catch(mysqlpp::BadQuery &ex)
		{
			MessageBox(NULL, ex.what(), "Error", MB_OK);
		}
		catch(...)
		{
			MessageBox(NULL, "There was an error, try restarting application.", "Error", MB_OK);
		}
	}
	if (objectName == L"Product" && callbackName == L"Delete" && active == false) //Delete a product and reflect the changes to the application
	{
		try
		{
			active = true;
			if (MessageBox(NULL, "Are you sure you want to delete that product?", "Confirmation", MB_YESNOCANCEL)==IDYES)
			{
				string prodId = wstringToString(args[0].toString());
				deleteProduct(prodId);
				Awesomium::JSArguments args;
				SS.clear();
				args.clear();
				aws.myWebView->callJavascriptFunction(L"", L"reloadList", args);;
			}
			active = false;
		}
		catch (const mysqlpp::ConnectionFailed &ex) 
		{
			MessageBox(NULL, "Connection to the database failed, \n make sure it is running and restart the application", "Error", MB_OK);
		}
		catch(...)
		{
			MessageBox(NULL, "There was an error, try restarting application.", "Error", MB_OK);
		}
	}
}



void Resize (int width, int height)
{
	if (height <= 0)
		height = 1;

	int aspectratio = width / height;

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
}
//Win Main
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, //Here we actually Start the form and tell it what to do
				   LPSTR lpCmdLine, int iCmdShow)
{
	WNDCLASS wc;
	HWND hWnd;
	HDC hDC;
	HGLRC hRC;
	MSG msg;
	BOOL quit = FALSE; //Whether we are closing the application or not
	float theta = 0.0f;

	// register window class
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground = (HBRUSH)GetStockObject( BLACK_BRUSH );
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "thebusiness";
	RegisterClass( &wc );

	// create main window
	hWnd = CreateWindow( 
		"thebusiness", "The Information Management System", 
		WS_CAPTION | WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		0, 0, tWidth, tHeight, NULL, NULL, hInstance, NULL );

	EnableOpenGL( hWnd, &hDC, &hRC ); //Tell OpenGL to render our form for us
	// program main loop
	while ( !quit )
	{	
		if((GetAsyncKeyState(VK_CONTROL))&&(GetAsyncKeyState(0x050)))
		{
			ESCPOSDevice* cashRegister = new ESCPOSDevice();
			cashRegister->open();
			if (cashRegister->isOpen())
			{
				cashRegister->initPrinter();
				cashRegister->openCashDrawer();
				cashRegister->close();
			}
		}
		updateApplication(25); //How often the form will update to show changes / movements
		if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE )  )
		{

			// handle or dispatch messages
			if ( msg.message == WM_QUIT ) 
			{
				quit = TRUE;
			} 
			else 
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}

		} 
		else 
		{

			// OpenGL animation code goes here
			glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
			glClear( GL_COLOR_BUFFER_BIT ); 
			glPixelZoom(1.0f, -1.0f); //flip the picture
			glRasterPos2f(-1,1.0);
			const Awesomium::RenderBuffer* myBuffer = aws.myWebView->render(); //renderer for awesomium output
			if(myBuffer)
			{
				glDrawPixels(tWidth, tHeight,GL_BGRA, GL_UNSIGNED_BYTE, myBuffer->buffer); //render all the things!!
			}
			glEnd(); //end rendering

			SwapBuffers( hDC ); //swap the rendering buffers


		}

	}

	// shutdown OpenGL
	DisableOpenGL( hWnd, hDC, hRC );

	// destroy the window explicitly
	DestroyWindow( hWnd );

	return msg.wParam;

}

//Listen for callbacks from the Operating system
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{

	case WM_CREATE:
		return 0;

	case WM_CLOSE:
		PostQuitMessage( 0 );
		return 0;

	case WM_DESTROY:
		return 0;

	case WM_SIZE:
		if(wParam == SIZE_MAXIMIZED)
		{
			RECT rcClient, rcWind;
			GetClientRect(hWnd, &rcClient);
			GetWindowRect(hWnd, &rcWind);

			tWidth = rcClient.right;
			tHeight = rcClient.bottom;
			Resize(tWidth,tHeight);
			aws.myWebView->resize(tWidth,tHeight,FALSE);
			isMaxd = TRUE;
		}
		else if(wParam == SIZE_RESTORED && isMaxd == TRUE)
		{
			RECT rcClient, rcWind;
			GetClientRect(hWnd, &rcClient);
			GetWindowRect(hWnd, &rcWind);

			tWidth = rcClient.right;
			tHeight = rcClient.bottom;
			Resize(tWidth,tHeight);
			aws.myWebView->resize(tWidth,tHeight,FALSE);
		}
		return 0;

	case WM_MOUSEMOVE: //Use my mouse moved function and pass in the x and y values given by the Operating System
		mouseMoved(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam)); 
		break;

	case WM_LBUTTONDOWN:
		mousePressed(mouse.x, mouse.y, 1, 1);
		isMousePressed = TRUE;
		break;

	case WM_LBUTTONUP:
		mousePressed(mouse.x, mouse.y, 0, 1);
		isMousePressed = FALSE;
		break;

	case WM_RBUTTONDOWN:
		mousePressed(mouse.x, mouse.y, 1, 2);
		break;

	case WM_RBUTTONUP:
		mousePressed(mouse.x, mouse.y, 0, 2);
		break;

	case WM_MOUSEWHEEL:
		delta = GET_WHEEL_DELTA_WPARAM(wParam);
		aws.myWebView->injectMouseWheel(delta/2);
		break;

	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_CHAR:
	case WM_SYSCHAR:
		{
			if ( wParam != 13) //Used to debug whether the Return key is being pressed or not
			{
				Awesomium::WebKeyboardEvent keyEvent(message, wParam, lParam);
				aws.myWebView->injectKeyboardEvent(keyEvent);
			}
			if (wParam == 13)
			{
				Awesomium::WebKeyboardEvent keyEvent(message, wParam, lParam);
				aws.myWebView->injectKeyboardEvent(keyEvent);
			}
			break;
		}

	default:
		return DefWindowProc( hWnd, message, wParam, lParam );

	}
	return 0;
}

// Enable OpenGL

void EnableOpenGL(HWND hWnd, HDC * hDC, HGLRC * hRC)
{
	PIXELFORMATDESCRIPTOR pfd;
	int format;

	// get the device context (DC)
	*hDC = GetDC( hWnd );

	// set the pixel format for the DC
	ZeroMemory( &pfd, sizeof( pfd ) );
	pfd.nSize = sizeof( pfd );
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;
	format = ChoosePixelFormat( *hDC, &pfd );
	SetPixelFormat( *hDC, format, &pfd );
	// create and enable the render context (RC)
	*hRC = wglCreateContext( *hDC );
	wglMakeCurrent( *hDC, *hRC );

}

// Disable OpenGL

void DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC)
{
	wglMakeCurrent( NULL, NULL );
	wglDeleteContext( hRC );
	ReleaseDC( hWnd, hDC );
}

void updateApplication(int sleepTime)
{
	Sleep(sleepTime); //Tell the application to sleep for x amount of Milliseconds
	Awesomium::WebCore::Get().update(); //Get the updated output from awesomium
}
string ExePath() //Used to find the directory path of the application run point
{
	char buffer[MAX_PATH];
	GetModuleFileName( NULL, buffer, MAX_PATH );
	string::size_type pos = string( buffer ).find_last_of( "\\/" );
	return string( buffer ).substr( 0, pos);
}
void mousePressed(int x, int y, int up, int type) //Used to differentiate the types of mouse actions
{
	if (up == 1 && type == 1)
	{
		aws.myWebView->injectMouseDown(Awesomium::LEFT_MOUSE_BTN); //Send the action to Awesomium
	}
	else if (up == 0 && type == 1)
	{
		aws.myWebView->injectMouseUp(Awesomium::LEFT_MOUSE_BTN);
	}
	else if (up == 1 && type == 2)
	{
		aws.myWebView->injectMouseDown(Awesomium::RIGHT_MOUSE_BTN);
	}
	else if (up == 0 && type == 2)
	{
		aws.myWebView->injectMouseUp(Awesomium::RIGHT_MOUSE_BTN);
	}

}
void mouseMoved(int x, int y) //Tell Awesomium where the mouse is
{
	aws.myWebView->injectMouseMove(x, y);
}
string openfilename() { //Open file dialog, return the file details
	char *filter = "All Files (*.*)\0*.*\0";
	HWND owner = NULL;
	OPENFILENAME ofn;
	char fileName[MAX_PATH] = "";
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = owner;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "";
	string fileNameStr;
	if ( GetOpenFileName(&ofn) )
		fileNameStr = fileName;
	return fileNameStr;
}
void getProds() //Used to get the product details from the database and make them usable
{
	// Connect to the database.
	SS.clear();
	mysqlpp::Connection conn; // new MySQL connection
	if (conn.connect(db, server, //connect to the database, if successful go forward
		user, pass)) 
	{
		mysqlpp::Query query = conn.query("select * from products"); //Database query to be executed
		if (mysqlpp::StoreQueryResult res = query.store()) //Store the result of the query
		{
			int as = res.num_rows(); //hold how many rows were returned
			for (size_t i = 0; i < res.num_rows(); i++) 
			{
				//Store the returned database rows to temporary strings
				UINT tempId = res[i]["ID"];
				string tempPid = res[i]["Product_ID"];
				string tempName = res[i]["Product_Name"];
				string tempDesc = res[i]["Product_Description"];
				string tempCat = res[i]["Product_Category"];
				double tempPrice = res[i]["Product_Price"];
				UINT tempStock = res[i]["Product_Stock"];
				//convert to a product class type
				product temp(tempId,tempPid,tempName,tempDesc,tempCat,tempPrice,tempStock);
				//store the product in a product vector
				SS.push_back(temp);
			}
		}
	}
	else 
	{
		cerr << "DB connection failed: " << conn.error() << endl;
	}
	conn.disconnect(); //Stop the database connection
}
void getProd(string iD)
{
	//Connect to the database.
	mysqlpp::Connection conn;
	if (conn.connect(db, server,
		user, pass)) 
	{
		mysqlpp::Query query = conn.query("select * from products where Product_ID='"+ iD +"'");
		if (mysqlpp::StoreQueryResult res = query.store()) 
		{
			int as = res.num_rows();
			for (size_t i = 0; i < res.num_rows(); i++) 
			{
				//Store the returned database rows to temporary strings
				UINT tempId = res[i]["ID"];
				string tempPid = res[i]["Product_ID"];
				string tempName = res[i]["Product_Name"];
				string tempDesc = res[i]["Product_Description"];
				string tempCat = res[i]["Product_Category"];
				double tempPrice = res[i]["Product_Price"];
				UINT tempStock = res[i]["Product_Stock"];
				UINT tempQty = 1;
				//convert to a product class type
				product temp(tempId,tempPid,tempName,tempDesc,tempCat,tempPrice,tempStock, tempQty);
				int size = SS.size();
				int count=0; //counter to see if we can find another product
				//in the vector with the same id as temp
				if (size != 0)
				{
					for (int j = 0; j < size; j++)
					{
						if (SS[j].Id() == temp.Id())
						{
							SS[j].SetQTY(SS[j].QTY() + temp.QTY());
							count++;
						}
					}
					if (count == 0)
					{
						SS.push_back(temp);
					}
				}
				else
				{
					SS.push_back(temp);
				}
				//store the product in a product vector
			}
		}
	}
	else {
		cerr << "DB connection failed: " << conn.error() << endl;
	}
	conn.disconnect();
}
void newProd(string prodID, string prodName, string prodDesc, string prodCat, double prodPrice, UINT prodStock)
{
	// Connect to the database.
	mysqlpp::Connection conn;
	if (conn.connect(db, server,
		user, pass)) 
	{
		mysqlpp::Query query = conn.query("INSERT INTO `thebusiness`.`products` (`Product_ID`, `Product_Name`, `Product_Description`, `Product_Category`, `Product_Price`, `Product_Stock`) VALUES ('"+ prodID +"','"+ prodName +"','"+ prodDesc +"','"+ prodCat +"','"+ doubleToString(prodPrice) +"','"+ intToString(prodStock) +"');");
		query.execute();
	}
	conn.disconnect();
}
void updateStock(int ID, int Stock)
{
	// Connect to the database.
	mysqlpp::Connection conn;
	if (conn.connect(db, server,
		user, pass)) 
	{
		mysqlpp::Query query = conn.query("UPDATE `thebusiness`.`products` SET `Product_Stock`='"+ intToString(Stock) +"' WHERE `ID`='"+ intToString(ID) +"';");
		query.execute();
	}
	conn.disconnect();
}
void newOrder(std::string date, double Price, int prodID, std::string Time, int orderID, int qty, double eftpos, double cash)
{
	int stock;
	// Connect to the database.
	mysqlpp::Connection conn;
	if (conn.connect(db, server,
		user, pass)) 
	{
		mysqlpp::Query query = conn.query("SELECT * FROM `thebusiness`.`products` WHERE `products`.`ID`="+intToString(prodID)+";");
		if (mysqlpp::StoreQueryResult res = query.store()) 
		{
			int as = res.num_rows();
			for (size_t i = 0; i < res.num_rows(); i++) 
			{
				stock = res[0]["Product_Stock"];
			}
		}
		if (stock >= qty)
		{
			mysqlpp::Query q = conn.query("INSERT INTO `thebusiness`.`orders` (`Order_Date`, `Order_Price`, `Prod_ID`, `Order_Time`, `Order_ID`, `Qty`) VALUES ('"+ date +"', '"+doubleToString(Price)+"', '"+ intToString(prodID)+"', '"+ Time +"', '"+ intToString(orderID) +"', '"+ intToString(qty) +"');");
			q.execute();
			mysqlpp::Query qr = conn.query("INSERT INTO `thebusiness`.`payment` (`Order_ID`,`Eftpos`,`Cash`) VALUES ('"+intToString(orderID)+"','"+doubleToString(eftpos)+"','"+doubleToString(cash)+"');");
			qr.execute();
			updateStock(prodID, stock-qty);
			cou++;
		}
	}
	conn.disconnect();
}
int getLastID()
{
	// Connect to the database.
	mysqlpp::Connection conn;
	int lastID;
	if (conn.connect(db, server,
		user, pass)) 
	{
		mysqlpp::Query query = conn.query("SELECT * FROM `thebusiness`.`orders` INNER JOIN `thebusiness`.`products` ON `orders`.`Prod_ID`=`products`.`ID` ORDER BY `orders`.`Order_ID` desc;");
		if (mysqlpp::StoreQueryResult res = query.store()) 
		{
			int as = res.num_rows();
			for (size_t i = 0; i < res.num_rows(); i++) 
			{
				lastID = res[0]["Order_ID"];
			}
		}
	}
	conn.disconnect();
	return lastID;
}
void getCategory()
{
	mysqlpp::Connection conn;
	string category;
	int alt;
	if (conn.connect(db, server,
		user, pass)) 
	{
		mysqlpp::Query query = conn.query("SELECT * FROM `thebusiness`.`categories`;");
		if (mysqlpp::StoreQueryResult res = query.store()) 
		{
			int as = res.num_rows();
			for (size_t i = 0; i < res.num_rows(); i++) 
			{
				category = res[i]["Name"];
				alt = res[i]["AltCode"];
				CatNames.push_back(category);
				CatAlts.push_back(alt);
			}
		}
	}
	conn.disconnect();
}
void purchaseList(string date)
{
	// Connect to the database.
	mysqlpp::Connection conn;
	if (conn.connect(db, server,
		user, pass)) 
	{
		mysqlpp::Query query = conn.query("SELECT * FROM `thebusiness`.`orders` INNER JOIN `thebusiness`.`products` ON `orders`.`Prod_ID`=`products`.`ID` INNER JOIN `thebusiness`.`payment` ON `orders`.`Order_ID`=`payment`.`Order_ID` WHERE `thebusiness`.`orders`.`Order_Date` = '"+ date +"' AND (`thebusiness`.`payment`.`Eftpos` + `thebusiness`.`payment`.`Cash`) >'0' ORDER BY `thebusiness`.`orders`.`Order_ID` DESC;");
		if (mysqlpp::StoreQueryResult res = query.store()) 
		{
			int as = res.num_rows();
			for (size_t i = 0; i < res.num_rows(); i++) 
			{
				string tempDate = res[i]["Order_Date"];
				string tempTime = res[i]["Order_Time"];
				string tempName = res[i]["Product_Name"];
				string tempCat = res[i]["Product_Category"];
				double tempPrice = res[i]["Order_Price"];
				double tempEftpos = res[i]["Eftpos"];
				double tempCash = res[i]["Cash"];
				int tempOrderId = res[i]["Order_ID"];
				string tempProdId = res[i]["Product_ID"];
				int tempQty = res[i]["Qty"];

				orderItem t (tempDate,tempTime,tempName,tempCat,tempPrice,tempEftpos,tempCash,tempOrderId,tempProdId,tempQty);
				OrderItems.push_back(t);
			}
		}
	}
	conn.disconnect();
}
void deleteProduct(string prodId)
{
	mysqlpp::Connection conn;
	if (conn.connect(db, server,
		user, pass)) 
	{
		mysqlpp::Query query = conn.query("DELETE FROM `thebusiness`.`products` WHERE `ID`='"+prodId+"';");
		query.execute();
	}
	conn.disconnect();
}
//Conversion methods 
std::string intToString(int i)
{
	std::stringstream ss;
	std::string s;
	ss << i;
	s = ss.str();

	return s;
}

std::string doubleToString(double i)
{
	std::stringstream ss;
	ss << i;
	std::string s;
	s = ss.str();
	return s;
}
string wstringToString(std::wstring b)
{
	string a;
	a.assign(b.begin(), b.end());
	return a;
}
double wstringToDouble(std::wstring b)
{
	wstringstream ss;
	ss << b;
	double a;
	ss >> a;
	return a;
}
int wstringToint(std::wstring b)
{
	wstringstream ss;
	ss << b;
	int a;
	ss >> a;
	return a;
}
UINT wstringToUint(std::wstring b)
{
	wstringstream ss;
	ss << b;
	UINT a;
	ss >> a;
	return a;
}
//Hows the brain feeling after trying to absorb all of that?