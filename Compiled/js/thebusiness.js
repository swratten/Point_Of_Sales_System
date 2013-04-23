//Global Variables
var data = new Array();
var catData = new Array();
var orderData = new Array();
var time = 0;
var purchase = false;
var totalprice;
var remaining;
var itemId;
var newStock;
var index;
var update = false;
var searching = false;
var list = false;
var newid;
var pic1;
var pic2;
var pic1w;
var pic1h;
var pic2w 
var	pic2h;
var urlimg;
var picName;
var categ;
var eftpos;
var cash;
var disabled = new Array();

function setLastID(lastID) //Function to set the last Order ID
{
	newid = lastID;
	var ind = data.length;
	for (var i = 0; i < ind; i++)
	{
		var d = new Date(); //New Date variable
		
		//Create a date variable with NZ Standard time / Date
		var month = d.getMonth() + 1;
		var day = d.getDate();
		
		var date = (('' + day).length < 2 ? '0' : '') + day + '-' +
			(('' + month).length < 2 ? '0' : '') + month + '-' +
			d.getFullYear();
		
		var hour = d.getHours();
		var minute = d.getMinutes();
		var seconds = d.getSeconds();
		var time = hour + ':' + minute + ':' + seconds;
		//Send the new Order to c++
		Orders.newOrder(date, data[i].ProdPrice, data[i].ID, time, newid, data[i].Qty,eftpos,cash);
		//Reset Variables
		eftpos=undefined;
		cash=undefined;
		purchase=false;
	}
	Orders.Print();
}
function checkTime(Time) //Function to prevent Clicking Spam
{
	if (Time == null)
	{
		Time = 1000;
	}
	temp = new Date().getTime();
	if (temp - time < Time)
	{
		return false;
	}
	time = new Date().getTime();
	return true;
}
//Create a new product using the Product class
function newProd()
{
	newProduct.Create($('#prodID').val(), $('#prodName').val(), $('#prodDesc').val(), categ, $('#prodPrice').val(), $('#prodStock').val());
}
//Product Class
function product(id, prodId, prodName, prodDesc, prodCat, prodPrice, prodStock, qTy, imgUrl)
{
	this.ID = id;
	this.ProdId = prodId;
	this.ProdName = prodName;
	this.ProdDesc = prodDesc;
	this.ProdCat = prodCat;
	this.ProdPrice = prodPrice;
	this.ProdStock = prodStock;
	this.Qty = qTy;
	this.ImgUrl = imgUrl;
}
//Category Class
function category(name,alt)
{
	this.Name = name + ' - ' + alt;
}
//Clear function to reset loads of variables
function clear()
{
	if (list == true) //check whether we are listing products
	{
		$("#listbox").jqxListBox( //clear the listbox
		{
			source : null
		}
		);
		list = false;
	}
	if (list != true)
	{
		$('#search').hide(); //Hide a Div
	}
	if (purchase != true)
	{
		$("#listbox").jqxListBox(
		{
			source : null
		}
		);
	}
	$('#cash').attr('checked',false); //change the checked property of a checkbox
	$('#eftpos').attr('checked',false);
	$('#Calendar').hide();
	$('#pics').html('');
	$('#searchField').val('');
	//Clear the Validator messages (they stay there otherwise)
	$('#testForm').jqxValidator('hideHint', '#prodID');
	$('#testForm').jqxValidator('hideHint', '#prodName');
	$('#testForm').jqxValidator('hideHint', '#prodPrice');
	$('#testForm').jqxValidator('hideHint', '#prodStock');
	//Clear Arrays
	data.length = 0;
	catData.length=0;
	orderData.length=0;
	$('#newProd').html('');
	$('#newProd').hide();
	$("#ContentPanel").html('');
	$('#amount').val('');
	if (purchase != true)
	{
		$("#listbox").jqxListBox(
		{
			source : null
		}
		);
		ProductClear.Clear();
		$('#payment').hide();
		remaining = '';
		$('#id').val('');
		$('#pid').val('');
		$("#listbox").hide();
		$("#total").hide();
		$("#ContentPanel").hide();
	}
}
//Function to load products
function loadProduct(id, prodid, name, desc, cate, price, stock, qty, img)
{
	price = price.toFixed(2); //Set the price variable to 2 decimal points and perform rounding
	var prod = new product(id, prodid, name, desc, cate, price, stock, qty, img);
	data.push(prod);
}
//Function to reload purchases
function reloadPurchase()
{
	purchase = true;
	clear();
	$("#total").show();
	$('#id').show();
	$("#listbox").show();
	$("#ContentPanel").show();
	$('#remaining').text('Remaining balance:');
	var pid = $("#id").val();
	Products.Load(pid);
	return false;
}
//Function to reload the listbox items
function reloadList()
{
if (checkTime() == true)
		{
			clear();
			list = true;
			$('#search').show();
			$('#id').val('');
			$("#listbox").show();
			$("#ContentPanel").show();
			ProductList.List();
			return false;
		}
}
//Function to create a new product
function newPrduct()
{
	//add the css classes to the text-input
	$('.text-input').addClass('jqx-input');
	$('.text-input').addClass('jqx-rc-all');
	//Set the source for the widget
	var source =
	{
		localdata : catData,
		datatype : "array"
	};
	//Adapt the data for the widget
	var dataAdapter = new $.jqx.dataAdapter(source);
	//create a new combobox widget using the data
	$('#prodCat').jqxComboBox({ selectedIndex: 0, theme: 'dropdown', source: dataAdapter, itemHeight: 20, height: 25, width: 200, displayMember: "Name", valueMember: "Name", enableBrowserBoundsDetection: true, searchMode: 'containsignorecase', autoComplete: true,
                renderer: function (index, label, value) {
                    var datarecord = catData[index];
                    var table = '<table style="min-width: 150px;"><tr><td style="width: 5px;" rowspan="1">'+ datarecord.Name +'</td></tr></table>';
                    return table;
                }
    });
	//Bind the select event to the combo box
	$('#prodCat').bind('select', function (event) {
	var args = event.args;
	var item = $('#prodCat').jqxComboBox('getItem', args.index);
	if (item != null) {
	categ = item.label;
	}
	});
	//a new jqx widget button
	$('#sendButton').jqxButton(
	{
		width : 60,
		height : 25,
		theme : 'classic'
	}
	);
	//Bind the click event to the button
	$('#sendButton').bind('click', function ()
	{
		var Pp = $('#prodPrice').val();
		$('#prodPrice').val(Pp.replace('-',''));
		var Ps = $('#prodStock').val();
		$('#prodStock').val(Ps.replace('-',''));
		//validate the input on the form
		$('#testForm').jqxValidator('validate');
	}
	);
	//Create a new validator for the form
	//Set up the rules for the validator so it knows what is valid 
	$('#testForm').jqxValidator(
	{ 
		rules : [
			{
				input : '#prodID',
				message : 'Product ID is required!',
				action : 'keyup, blur',
				rule : 'required'
			},
			{
				input : '#prodID',
				message : 'You must only enter numbers!',
				action : 'keyup',
				rule : 'number'
			},
			{
				input : '#prodID',
				message : 'Product ID must be at least 5 numbers long',
				action : 'keyup',
				rule : 'minLength=5'
			},
			{
				input : '#prodName',
				message : 'Product Name is required!',
				action : 'keyup',
				rule : 'required'
			},
			{
				input : '#prodPrice',
				message : 'Product Price is required!',
				action : 'keyup',
				rule : 'required'
			},
			{
				input : '#prodPrice',
				message : 'You must only enter numbers!',
				action : 'keyup',
				rule : 'number'
			},
			{
				input : '#prodStock',
				message : 'Product Stock is required!',
				action : 'keyup',
				rule : 'required'
			},
			{
				input : '#prodStock',
				message : 'You must only enter numbers!',
				action : 'keyup',
				rule : 'number'
			}
		],
		theme : 'classic'
	}
	);
	//Reset the form if the values were successfully validated after submission
	$('#testForm').bind('validationSuccess', function ()
	{
		newProd();
		$('#prodID').val('');
		$('#prodName').val('');
		$('#prodPrice').val('');
		$('#prodStock').val('');
		$('#prodDesc').val('');
	}
	);
	$("#prodCat").jqxComboBox('selectIndex', 1 );
	$("#prodCat").jqxComboBox('selectIndex', 0 );
}
function enable(id, rowid) //enable a div
{
	$(id).show();
	$('#click').show();
	itemId = rowid;
}
//Update stock function
function updateStock()
{
	update = true;
	index = $("#listbox").jqxListBox('getSelectedIndex');
	var newstock = $('#' + itemId).val();
	Stock.Update(itemId, newstock); //call the update stock function
	$("#listbox").jqxListBox('beginUpdate');
	$("#listbox").jqxListBox('clear');
	data.length = 0;
	ProductList.List();
	$("#listbox").jqxListBox('endUpdate');
}
function search(text) //Function to call the search callback
{
	Search.Execute(text);
}
//Get the JSON data back from the google image AJAX server , then read its image URL so we can load it to the form
//Specify that we want 2 images so we can choose which one we want
function getJSON(name)
{
	$('#pics').html('');
	$.getJSON('https://ajax.googleapis.com/ajax/services/search/images?v=1.0&q=%22' + name + '%22&rsz=5&format=js&callback=?', function (data)
	{
		$('#pics').append('<img src="' + data.responseData.results[0].unescapedUrl + '" style="max-height:250px; max-width:200px;" onclick="if($(\'#prodID\').val().length>4&&$(\'#prodName\').val().length>0){if($(this).is(\'.selected\')){$(this).removeClass(\'selected\');}else{if($(\'#secondPic\').is(\'.selected\')){$(\'#secondPic\').removeClass(\'selected\');}$(this).addClass(\'selected\'); saveImage(1);}}" id="firstPic"></img>');
		$('#pics').append('<img src="' + data.responseData.results[1].unescapedUrl + '" style="max-height:250px; max-width:200px;" onclick="if($(\'#prodID\').val().length>4&&$(\'#prodName\').val().length>0){if($(this).is(\'.selected\')){$(this).removeClass(\'selected\');}else{if($(\'#firstPic\').is(\'.selected\')){$(\'#firstPic\').removeClass(\'selected\');}$(this).addClass(\'selected\'); saveImage(2);}}" id="secondPic" ></img>');
		pic1 = data.responseData.results[0].unescapedUrl;
		pic1w = data.responseData.results[0].width;
		pic1h = data.responseData.results[0].height;
		pic2 = data.responseData.results[1].unescapedUrl;
		pic2w = data.responseData.results[1].width;
		pic2h = data.responseData.results[1].height;
	}
	);
	
}
//Order Class
function Order(orderDate, orderTime, orderProduct, orderCategory, orderPrice, orderEftpos, orderCash, orderId, orderProductId, orderQty)
{
	this.OrderDate = orderDate;
	this.OrderTime = orderTime;
	this.OrderProduct = orderProduct;
	this.OrderCategory = orderCategory;
	this.OrderPrice = orderPrice;
	this.OrderEftpos = orderEftpos;
	this.OrderCash = orderCash;
	this.OrderId = orderId;
	this.OrderProductId = orderProductId;
	this.OrderQty = orderQty;
}
//Load Order Function
function loadOrder(date, time, prod, cat, price, eft, cas, id, prodid, qty)
{
	var order = new Order(date, time, prod, cat, price, eft, cas, id, prodid, qty);
	orderData.push(order);
}
//List the order details
function listOrder()
{
	//Arrays to sort the order details
	var selectData = new Array();
	var ids = new Array();
	var time = new Array();
	var epos = new Array();
	var cas = new Array();
	var cha = new Array();
	var etot = 0;
	var ctot = 0;
	var chtot = 0;
	var total = new Array();
	var oTotal = new Array();
	var count = orderData.length;
	var idcount;
	var to = 0;
	var daytotal = 0;
	//Work out the total price of the order
	if(count == 0)
	{
		$('#daySales').html('Total Day Sales: $' + daytotal.toFixed(2));
	}
	for( var i=0;i<count;i++)
	{
		if(idcount != orderData[i].OrderId)
		{
			total.length=0;
			idcount = orderData[i].OrderId;
			var e = orderData[i].OrderEftpos;
			epos.push(e);
			etot = etot + (orderData[i].OrderEftpos);
			ctot = ctot + (orderData[i].OrderCash);
			to = to + (orderData[i].OrderCash + orderData[i].OrderEftpos);
			var c = orderData[i].OrderCash;
			cas.push(c);
			total.push(orderData[i].OrderPrice * orderData[i].OrderQty); //push the order total price
			daytotal = daytotal + (orderData[i].OrderPrice * orderData[i].OrderQty);

			chtot = to - daytotal;
			
		}
		else
		{
			ctot = ctot + (orderData[i].OrderCash);
			to = to + (orderData[i].OrderCash + orderData[i].OrderEftpos);
			etot = etot + (orderData[i].OrderEftpos);
			total.push(orderData[i].OrderPrice * orderData[i].OrderQty);
			daytotal = daytotal + (orderData[i].OrderPrice * orderData[i].OrderQty);
			chtot = to - daytotal;
			
		}
		$('#daySales').html('Total Day Sales(- Change): $' + daytotal.toFixed(2) + ' Total Eftpos: $' + etot.toFixed(2) + ' Total Cash: $' + ctot.toFixed(2) + ' Total Change: $' + chtot.toFixed(2));
		if( (i ==(count-1)) || (orderData[i + 1].OrderId != idcount))
			{
				var oChange = 0;
				for(var e =0;e<total.length;e++)
				{
					oChange = oChange + total[e];
				}
				var payTotal = orderData[i].OrderEftpos + orderData[i].OrderCash;
				oTotal.push(oChange);
				oChange = payTotal - oChange;
				cha.push(oChange);
			}
		var tm = orderData[i].OrderId;
		ids.push(tm);
		var tt = orderData[i].OrderTime;
		time.push(tt);
	}
	uniqueArray = ids.filter(function(elem, pos) {
    return ids.indexOf(elem) == pos;
	});
	uArray = time.filter(function(elem, pos) {
    return time.indexOf(elem) == pos;
	});
	//Time to give the data to the widget
	for(var i=0;i< uniqueArray.length; i++)
	{
		var row ={};
		row["OrderId"] = uniqueArray[i];
		row["OrderName"] = "Order " + (i + 1);
		row["OrderTime"] = uArray[i];
		row["OrderEftpos"] = epos[i];
		row["OrderCash"] = cas[i];
		row["OrderChange"] = cha[i];
		row["OrderTotal"] = oTotal[i];
		selectData[i] = row;
	}
	var srce =
			{
                localdata: selectData,
                datatype: "array"
            };
            var dataAdapt = new $.jqx.dataAdapter(srce);
			//create the top order grid
            $("#ordergrid").jqxGrid(
            {
                width: 720,
				height: 150,
                source: dataAdapt,
                theme: 'classic',
                columnsresize: true,
                columns: [
                  { text: 'Order ID', dataField: 'OrderId', width: 100 },
				  { text: 'Order Name', dataField: 'OrderName', width: 100 },
				  { text: 'Order Time', dataField: 'OrderTime', width: 100 },
				  { text: 'Order Total', dataField: 'OrderTotal', width: 100, cellsformat: 'c2' },
				  { text: 'Eftpos', dataField: 'OrderEftpos', width: 100, cellsformat: 'c2' },
				  { text: 'Cash', dataField: 'OrderCash', width: 100, cellsformat: 'c2' },
				  { text: 'Change', dataField: 'OrderChange', width: 100, cellsformat: 'c2' }
                ]
            });

			var source =
			{
                localdata: orderData,
                datatype: "array"
            };
            var dataAdapter = new $.jqx.dataAdapter(source);
			$("#ordergrid").bind('rowselect', function (event) {
                var OrderId = event.args.row.OrderId;
                var records = new Array();
                var length = orderData.length;
                for (var i = 0; i < length; i++) {
                    var record = orderData[i];
                    if (record.OrderId == OrderId) {
                        records[records.length] = record;
                    }
                }
                var dataSource = {
                    localdata: records,
					datatype: "array"
                }
                var adapter = new $.jqx.dataAdapter(dataSource);
        
                // update data source.
                $("#cal_grid").jqxGrid({ source: adapter });
            });
			//The bottom datagrid
            $("#cal_grid").jqxGrid(
            {
                width: 1070,
				height: 150,
                theme: 'classic',
                columnsresize: true,
                columns: [
                  { text: 'Order Date', dataField: 'OrderDate', width: 100 },
                  { text: 'Order Time', dataField: 'OrderTime', width: 100 },
                  { text: 'Product', dataField: 'OrderProduct', width: 180 },
                  { text: 'Category', dataField: 'OrderCategory', width: 180, cellsalign: 'left' },
                  { text: 'Price', dataField: 'OrderPrice', width: 80, cellsalign: 'left', cellsformat: 'c2' },
				  { text: 'Order ID', dataField: 'OrderId', cellsalign: 'left', minwidth: 60 },
				  { text: 'Product ID', dataField: 'OrderProductId', cellsalign: 'left', minwidth: 100 },
				  { text: 'Quantity', dataField: 'OrderQty', cellsalign: 'left', minwidth: 50 }
                ]
            });
		$('#ordergrid').jqxGrid('selectrow', 0);
}
//Function to load the images that we have googled
function loadImage(id)
{
	$('#pics').html('');
	$('#pics').append('<img src="'+id+'" style="max-height:250px; max-width:150px;"></img>');
}
//Function to save the image we got from the google image search
function saveImage(num)
{
	if(num==1)
	{
		Picture.Get(pic1w,pic1h,pic1,picName);
	}
	if(num==2)
	{
		Picture.Get(pic2w,pic2h,pic2,picName);
	}
}
//Image variables
var imgHeight;
var imgWidth;
var url;
//find the image width and height
function findHHandWW() {
imgHeight = this.height;imgWidth = this.width;
saveUrlImage(); //call the save url image function
return true;
}
// call the get picture function
function saveUrlImage()
{
	Picture.Get(imgWidth,imgHeight,url,"blah.png");
}
//Display an image
function showImage(imgPath) {
var myImage = new Image();
myImage.name = imgPath;
url = imgPath;
myImage.onload = findHHandWW;
myImage.src = imgPath;
}
//Used to check an item in the purchase to see if it has enough stock to be bought
function check(ID)
{
	disabled = jQuery.grep(disabled, function (value)
		{
			return value != ID;
		}
		);
	if (disabled.length == 0)
	{
		$('#enter').removeAttr("disabled");
	}
}
//Function to load the categories
function loadCategory(name,alt)
{
	var cat = new category(name,alt);
	catData.push(cat);
}
//Function to display products in the listbox widget
function listbox(totalPrice)
{
	var source =
	{
		localdata : data,
		datatype : "array"
	};
	var dataAdapter = new $.jqx.dataAdapter(source,
		{
			formatData : function (data)
			{
				data.ProdId_startsWith = $("#searchfield").val();
				return data;
			}
		}
		);
	$('#listbox').bind('select', function (event)
	{
		var container = $('<div style="margin: 5px;"></div>');
		var leftcolumn = $('<div style="float: left;"></div>');
		var imagecontainer = $('<div style="margin-left: 10px; max-height:250px; max-width:200px; float: right; overflow:hidden;"></div>');
		container.append(leftcolumn);
		container.append(imagecontainer);
		var datarecord = data[event.args.index];
		var prodid = "<div style='margin: 10px;'><b>Product ID:</b> " + datarecord.ProdId + "</div>";
		var prodname = "<div style='margin: 10px;'><b>Product Name:</b> " + datarecord.ProdName + "</div>";
		var proddesc = "<div style='margin: 10px;'><b>Product Description:</b> " + datarecord.ProdDesc + "</div>";
		var prodcat = "<div style='margin: 10px;'><b>Product Category:</b> " + datarecord.ProdCat + "</div>";
		var prodprice = "<div style='margin: 10px;'><b>Product Price:</b> $" + datarecord.ProdPrice + "</div>";
		var prodstock = "<div style='margin: 10px;'><b>Product Stock:</b> " + datarecord.ProdStock + '</div>';
		imagecontainer.append('<img src="'+ datarecord.ImgUrl +'" style="max-height:250px; max-width:200px; border: 1px black solid;" />');
		if (list == true) //If we are just listing products we can delete a product / update its stock
		{
			var update = "<div style='margin: 10px;'>" + '<input type="checkbox" onclick="if($(this).is(\':checked\')){enable(\'#\'+' + datarecord.ID + ',' + datarecord.ID + ');}else{$(\'#\'+' + datarecord.ID + ').hide(); $(\'#click\').hide();}" /> Update Stock: <br /><input type="textbox" id=' + datarecord.ID + ' style="display :none;" class="prod" /><button type="button" id="click" onclick="updateStock();" style="display: none;">Enter</button></div>';
			var delet = " <div style='margin: 10px;'>" + "<button type='button' onclick='Product.Delete("+ datarecord.ID +");'>Delete Product</button> </div>";
		}
		$(leftcolumn).append(prodid);
		$(leftcolumn).append(prodname);
		$(leftcolumn).append(proddesc);
		$(leftcolumn).append(prodcat);
		$(leftcolumn).append(prodprice);
		$(leftcolumn).append(prodstock);
		if (list == true)
		{
			$(leftcolumn).append(update);
			$(leftcolumn).append(delet);
		}
		$("#ContentPanel").html(container.html());
	}
	);
	//Display the listbox
	$("#listbox").jqxListBox(
	{
		source : dataAdapter,
		width : '30%',
		height : '62%',
		theme : 'classic',
		renderer : function (index, label, value)
		{
			var datarecord = dataAdapter.records[index];
			if (purchase == true) //If we are wanting to process an order
			{
			//Figure out whether we have enough stock for said products
				if ((datarecord.ProdStock - datarecord.Qty) >= 0)
				{
					var table = '<table style="min-width: 150px;"><tr><td>' + datarecord.ProdName + "</td></tr><tr><td>  $" + datarecord.ProdPrice + "    x" + datarecord.Qty + '    <button type="button" onclick="ProductRemove.Remove(' + datarecord.ID + ')">-</button>' + '</td></tr></table>';
				}
				if ((datarecord.ProdStock - datarecord.Qty) < 0)
				{
					var listed = false;
					var table = '<table style="min-width: 150px;"><tr><td>Not enough stock.  x' + datarecord.Qty + '    <button type="button" onclick="ProductRemove.Remove(' + datarecord.ID + '); check(' + datarecord.ID + ');">-</button>' + '</td></tr></table>'
						if (disabled.length > 0)
						{
							for (var i = 0; i < disabled.length; i++)
							{
								if (disabled[i] == datarecord.ID)
								{
									listed = true;
								}
							}
						}
						if (listed == false)
						{
							disabled.push(datarecord.ID);
						}
						$('#enter').attr("disabled", "disabled");
				}
			}
			else
			{
				var table = '<table style="min-width: 150px;"><tr><td>' + datarecord.ProdName + "</td></tr><tr><td>  $" + datarecord.ProdPrice + '</td></tr></table>';
			}
			return table;
		}
	}
	);
	$("#listbox").jqxListBox(
	{
		valueMember : 'ProdName'
	}
	);
	if (update == true) //If we have updated something select the index of the product we updated
	{
		$("#listbox").jqxListBox(
		{
			selectedIndex : index
		}
		);
	}
	if (update == false)
	{
		$("#listbox").jqxListBox(
		{
			selectedIndex : 0
		}
		);
	}
	$("#listbox").jqxListBox( 
	{
		searchMode : 'containsignorecase'
	}
	);
	if (purchase == false)
	{
		$("#listbox").jqxListBox(
		{
			//height : 350
		}
		);
	}
	if (purchase == true)
	{
		remaining = totalPrice.toFixed(2); //Update the remaining payment amount to 2 decimal places and round the amount
	}
	$("#id").val('$' + totalPrice.toFixed(2));
	purchase = false;
}
function clearPics()
{
	$('#pics').html('');
}
// get item value.
var itemValue;
//Jquery functions
$(function ()
{
	//Hide all of the divs to start off with
	$('#search').hide();
	$('#payment').hide();
	$('#newProd').hide();
	$('#total').hide();
	$('#id').hide();
	$('#Calendar').hide();
	// Dialog Link
	var me = this;
	$('#searchField').focus(function () //Tell the application we are searching when the textbox has focus
	{
		searching = true;
	}
	);
	$('#searchField').focusout(function ()  //Tell the application we have finished searching when the textbox loses focus
	{
		searching = false;
	}
	);
	$('#searchField').bind('keydown', function (event) //when the user types something into the search box
	{
		if (searching == true)
		{
			//time the input so we dont double up results
			if (me.timer)
				clearTimeout(me.timer);
			me.timer = setTimeout(function ()
				{
					$("#listbox").jqxListBox('beginUpdate');
					$("#listbox").jqxListBox('clear');
					data.length = 0;
					Search.Execute($('#searchField').val());
					$("#listbox").jqxListBox('endUpdate');
				}, 1);
		}
	}
	);
	//go to the new products area
	$('#new_prods').click(function ()
	{
		if (checkTime() == true)
		{
			clear(); //perform a clear
			$('#newProd').html('<form id="testForm" action="./">' +
				'<table class="register-table">' +
				'<tr>' +
				'<td><div id="prodCat" style="float:left;"></div></td>'+
				'<td><button type="button" style="float:left;" id="getPic" onclick="if($(\'#prodID\').val().length>4&&$(\'#prodName\').val().length>0){getJSON($(\'#prodName\').val()); picName=$(\'#prodID\').val() + \'.jpg\';}">Get Picture</button><button type="button" style="float:left;" id="fromDisk" onclick="if($(\'#prodID\').val().length>0){Picture.FromDisk($(\'#prodID\').val()+\'.jpg\');clearPics();}">Picture from drive</button></td>'+
				'</tr>' +
				'<tr>' +
				'<td>Product ID:</td>' +
				'<td><input type="text" id="prodID" class="text-input prod"/></td>' +
				'</tr>' +
				'<tr>' +
				'<td>Product Name:</td>' +
				'<td><input type="text" id="prodName" class="text-input prod"/></td>' +
				'</tr>' +
				'<tr>' +
				'<td>Product Description:</td>' +
				'<td><input type="text" id="prodDesc" class="text-input prod" /></td>' +
				'</tr>' +
				'<tr>' +
				'<td>Product Price:</td>' +
				'<td><input type="text" id="prodPrice" class="text-input prod" /></td>' +
				'</tr>' +
				'<tr>' +
				'<td>Product Stock:</td>' +
				'<td><input type="text" id="prodStock" class="text-input prod" /></td>' +
				'</tr>' +
				'</table>' +
				'<input type="button" value="Save" id="sendButton" />'+
				'</form>'
				);
			$('#newProd').show();
			Category.Get();
			imgHeight='';
			imgWidth='';
			
		}
	}
	);
	$('#home').click(function ()
	{
		if (checkTime() == true)
		{
			clear();
		}
	}
	);
	$('#enter').click(function ()
	{
		if (remaining == 0 || remaining < 0) //check whether there is any owed amount in a purchase
		{
		}
		
		else
		{
			//check which payment methods are being checked
			if((document.getElementById("cash").checked == true) || (document.getElementById("eftpos").checked == true))
			{
				if(document.getElementById("cash").checked == true)
				{
					if(cash===undefined)
					{
					//tell javascript that we are working with numbers instead of strings
					//So we can perform maths equations
						cash = $('#amount').val() /1; 
					}
					else
					{
						cash = cash /1 + $('#amount').val() /1;
					}
					cash = cash.toFixed(2);
					cash = cash.replace('-','');
				}
				if(document.getElementById("eftpos").checked == true)
				{
					if(eftpos===undefined)
					{
						eftpos = $('#amount').val() /1;
					}
					else
					{
						eftpos = eftpos /1 + $('#amount').val() /1;
					}
					eftpos = eftpos.toFixed(2);
					eftpos = eftpos.replace('-','');
				}
				var Amt = $('#amount').val();
				Amt = Amt.replace('-','');
				remaining = remaining - Amt;
				remaining = remaining.toFixed(2);
				$('#remaining').text('Remaining balance: $' + remaining);
				$('#cash').attr('checked',false);
				$('#eftpos').attr('checked',false);
				if (remaining <= 0)
				{
					$('#remaining').text('Transaction Change: $' + remaining);
					Orders.getLastID();
					$('#amount').val('');
				}
				$('#amount').val('');
			}
		}
	}
	);
	$('#void').click(function ()
	{
		if (checkTime() == true)
		{
			$('#amount').val('');
			$('#remaining').text('Remaining balance:');
			ProductClear.Clear();
			$("#listbox").jqxListBox(
			{
				source : null
			}
			);
			$("#ContentPanel").html('');
			$("#id").val('$0.00');
			$('#cash').attr('checked',false);
			$('#eftpos').attr('checked',false);
		}
	}
	);
	$('#new').click(function(){
	if (checkTime() == true)
		{
			$('#amount').val('');
			$('#remaining').text('Remaining balance:');
			ProductClear.Clear();
			$("#listbox").jqxListBox(
			{
				source : null
			}
			);
			$("#ContentPanel").html('');
			$("#id").val('$0.00');
			$('#cash').attr('checked',false);
			$('#eftpos').attr('checked',false);
			$("input[id='pid']").focus();
		}
	}
	);
	$('#list_purchases').click(function(){
		clear();
		$('#Calendar').show();
		//Create a new date picker so the user can choose which date they want to see purchases from
		$( "#datepicker" ).datepicker({dateFormat: "dd-mm-yy", onSelect: function() {orderData.length=0; Orders.List($(this).val()); $('#cal_grid').show(); $('#ordergrid').show(); $('#datepicker').datepicker( "hide" ); $('#datepicker').datepicker( "refresh" ); }});
		$( "#datepicker" ).datepicker( "getDate" );
		var d = new Date();
		
		var month = d.getMonth() + 1;
		var day = d.getDate();
		
		var date = (('' + day).length < 2 ? '0' : '') + day + '-' +
			(('' + month).length < 2 ? '0' : '') + month + '-' +
			d.getFullYear();
		$('#datepicker').val(date);
		Orders.List($('#datepicker').val());
	});
	$('#dialog_link').click(function ()
	{
		if (checkTime() == true)
		{
			clear();
			list = true;
			$('#search').show();
			$('#id').val('');
			$("#listbox").show();
			$("#ContentPanel").show();
			ProductList.List();
			return false;
		}
	}
	);
	$('#datepicker').focus(function(){
	$('#cal_grid').jqxGrid('clear');
	$('#cal_grid').hide();
	$('#ordergrid').hide();
	});
	$("#pid").keyup(function (e) {
    if (e.keyCode == 13) { //Check whether the return key was pressed (barcode scanner sends a return after sending barcode)
	purchase = true;
			clear();
        var pid = $("#pid").val();
		Products.Load(pid);
		$("#pid").val('');
    }
});

$('html').bind('keypress', function(e)
{
	if(purchase!=true)
	{
		if(e.keyCode == 13) //Check whether the return key was pressed
		{
			return false;
		}
	}
});
	$('#pdialog_link').click(function ()
	{
		if (checkTime() == true)
		{
			purchase = true;
			clear();
			$('#payment').show();
			$('#remaining').text('Remaining balance:');
			$("#total").show();
			$('#id').show();
			$("#listbox").show();
			$("#ContentPanel").show();
			Products.Load('');
			$("input[id='pid']").focus();
			return false;
		}
	}
	);
	
}
);