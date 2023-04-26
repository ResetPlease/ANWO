var value_markets = 0;
let markets = new Array();
var value_days = 0;
var value_product = 0;
var percent_up = 0;
var percent_down = 0;
var succes_data = null;
var current_iterator = 1;
var interval_id = 0;
var WareHouse = null;
var MegaTrash = null;
var NextDay_bool = false;
var Day = 0;
var product_list = null;
var current_product = new Array();
var current_null = new Array();
var current_null_bad = new Array();
$.get(
    "http://localhost:3422/product.txt",
    function(data){
        product_list = data.split("\n");
        console.log(product_list);
    }
);

class Info {
    constructor(){
        this.income = 0;
        this.lost = 0;
        this.order = 0;
        this.bad_product = 0;
        this.good_product = 0;
        this.intrash = 0;
        this.complete = 0;
    }
    cmpl(){this.complete += 1;}
    add(inc){this.income += inc;}
    dec(dd){this.lost += dd;}
    orders(){this.order += 1;}
    good(gd){this.good_product += gd;}
    bad(fall){this.bad_product += fall;}
    trash(tr){this.intrash += tr;}
}

var DayInfo = new Info();

class Market {
    constructor(img_url) {
        this.complete_orders = 0;
        this.all_orders = 0;
        this.img_url = "";
        this.market_name = "";
        this.market_id = 0;
        this.img_object = new Image();
        this.products = new Array();
        this.posx = 0;
        this.posy = 0;
        this.img_url = img_url;
        this.img_object.src = this.img_url;
        this.img_object.style = 
        `position: absolute;
        width: 200px;
        height:200px;`;
    }
}

function day_active(){
    if(current_iterator >= succes_data.split("\n").length){
        $("#End").modal("show");
        return;
    }
    if(NextDay_bool == true){
        return;
    }
    NextDay_bool = true;
    Day++;
    document.getElementById("day_info").innerHTML = "Day " + Day;
    for(var i =0; i<current_null.length; ++i){
        current_null[i] = 0;
    }
    for(var i =0; i<current_null_bad.length; ++i){
        current_null_bad[i] = 0;
    }
}

function WareHouse_click(){
    var wrh = document.getElementById("info_warehouse");
    wrh.innerHTML += '<canvas id="WareHouseGood" width="480" height="320"></canvas>';
    const ctx = document.getElementById('WareHouseGood').getContext("2d");
    const myChart = new Chart(ctx, {
        type: 'bar',
        data: {
            labels: current_product,
            datasets: [{
                label: '#Количество',
                data: current_null,
                backgroundColor: [
                    'rgba(255, 99, 132, 0.2)',
                ],
                borderColor: [
                    'rgba(255, 99, 132, 1)',
                ],
                borderWidth: 1
            }]
        },
        options: {
            scales: {
                y: {
                    beginAtZero: true
                }
            }
        }
    });
    wrh = document.getElementById("modal-info");
    wrh.innerHTML += '<canvas id="WareHouseBad" width="480" height="320"></canvas>';
    const ctx2 = document.getElementById('WareHouseBad').getContext("2d");
    const myChart2 = new Chart(ctx2, {
        type: 'bar',
        data: {
            labels: current_product,
            datasets: [{
                label: '#Количество',
                data: current_null_bad,
                backgroundColor: [
                    'rgba(255, 99, 132, 0.2)',
                ],
                borderColor: [
                    'rgba(255, 99, 132, 1)',
                ],
                borderWidth: 1
            }]
        },
        options: {
            scales: {
                y: {
                    beginAtZero: true
                }
            }
        }
    });
    $("#WareHouseModal").modal("show");
}

function position() {
    WareHouse = new Image();
    WareHouse.src = "https://i.ibb.co/XDvXwxH/warehouse-inc.png";
    WareHouse.setAttribute("onclick","WareHouse_click();");
    WareHouse.style =
    `   position: absolute;
        margin-left:45%;
        width: 250px;
        height:250px;
        margin-top: 25%;`;
    document.body.appendChild(WareHouse);
    MegaTrash = new Image();
    MegaTrash.src = "https://i.ibb.co/3mfpGv0/trash.png";
    MegaTrash.style =
     `  position:absolute;
        width:100px;
        height:100px;
        right:0;
        bottom:10px;`;
    document.body.appendChild(MegaTrash);
    for(var i = 0; i<value_markets; ++i){
        var newMarket = new Market("https://i.ibb.co/L5SH8S7/supermarket.png");
        newMarket.img_object.id = i;
        newMarket.market_id = i;
        newMarket.img_object.setAttribute("onclick","img_click(" + newMarket.market_id + ");");
        markets.push(newMarket);
    }
    var fst = value_markets;
    if(value_markets > 6){
        fst = 6;
    }
    var delta_img = (1920 - 250*fst - 50*(fst-1) ) / 2;
    var sec = delta_img;
    var nwk = value_markets-6;
    var t_sec = (1920 - 250*nwk - 50*(nwk-1))/2;
    for(var i = 0; i<value_markets; ++i){
        if(i > 5){  
            markets[i].img_object.style.left += t_sec + 'px';
            markets[i].img_object.style.top += "200px";
            markets[i].posx = t_sec;
            markets[i].posy = 200;
            t_sec += 300;
            document.body.appendChild(markets[i].img_object);
            continue;
        }
        markets[i].img_object.style.left += sec + 'px';
        markets[i].posx = sec;
        markets[i].img_object.style.top += 50 + "px";
        markets[i].posy = 50;
        sec += 300;
        document.body.appendChild(markets[i].img_object);
    }
}

function parse(){
    var db = succes_data.split("\n");
    db.push("Day");
    console.log("Current iterator: " + db[current_iterator]);
    if(current_iterator >= db.length){
        $("#End").modal("show");
        console.log("End interation");
        clearInterval(interval_id);
    const ctx3 = document.getElementById('myCanvas').getContext("2d");
    console.log("Income "+DayInfo.income);
    console.log("Lost "+DayInfo.lost);
    const myChart3 = new Chart(ctx3, {
        type: 'bar',
        data: {
            labels: ['Прибыль', 'Убыль'],
            datasets: [{
                label: '#Количество',
                data: [DayInfo.income, DayInfo.lost],
                backgroundColor: [
                    'rgba(153, 102, 255, 0.2)',
                    'rgba(255, 159, 64, 0.2)'
                ],
                borderColor: [
                    'rgba(153, 102, 255, 1)',
                    'rgba(255, 159, 64, 1)'
                ],
                borderWidth: 1
            }]
        },
        options: {
            scales: {
                y: {
                    beginAtZero: true
                }
            }
        }
    });
        return;
    }
    if(NextDay_bool == false){
        return;
    }
    if(db[current_iterator].split(" ")[0] == "Day"){
        $("#DayModal").modal("show");
        var DayModal = document.getElementById("info_day");
        DayModal.innerHTML = 
        `Заработано: ${DayInfo.income}<br>
         Потрачено: ${DayInfo.lost}<br>
         Всего заказов: ${DayInfo.order}<br>
         Из них выполнено: ${DayInfo.complete}<br>
         Списанных продуктов: ${DayInfo.intrash}<br>`;
        NextDay_bool = false;
    }
    else if(db[current_iterator].split(" ")[0] == "Bad"){
        var bad_name = Number(db[current_iterator].split(" ")[1]);
        var bad_val = Number(db[current_iterator].split(" ")[2]);
        current_null_bad[bad_name] = bad_val;
        DayInfo.bad(bad_val);
    }
    else if(db[current_iterator].split(" ")[0] == "Good"){
        var good_name = Number(db[current_iterator].split(" ")[1]);
        var good_val = Number(db[current_iterator].split(" ")[2]);
        current_null[good_name] = good_val;
        DayInfo.good(good_val);
    }
    else if(db[current_iterator].split(" ")[0] == "Income"){
        var income = Number(db[current_iterator].split(" ")[1]);
        DayInfo.add(income);
    }
    else if(db[current_iterator].split(" ")[0] == "Lost"){
        var lost = Number(db[current_iterator].split(" ")[1]);
        DayInfo.dec(lost);
    }
    else if(db[current_iterator].split(" ")[0] == "Send") {
        DayInfo.cmpl();
        var where = Number(db[current_iterator].split(" ")[1]);
        console.log("Market " + where);
        markets[where].complete_orders += 1;
        var box = create_box("box"+current_iterator);
        move_to(box, markets[where].img_object.offsetLeft, markets[where].img_object.offsetTop);
    }
    else if(db[current_iterator].split(" ")[0] == "Order"){
        DayInfo.orders();
        var who = Number(db[current_iterator].split(" ")[1]);
        markets[who].all_orders += 1;
        console.log("To WareHouse from " + who);
        var order = create_order(current_iterator, markets[who].img_object);
        move_to(order, WareHouse.offsetLeft, WareHouse.offsetTop, "order");
    }
    else if(db[current_iterator].split(" ")[0] == "Trash"){
        var who = Number(db[current_iterator].split(" ")[1]);
        var vl = Number(db[current_iterator].split(" ")[2]);
        DayInfo.trash(vl);
        console.log("To Trahs from Warehouse with " + who);
        var box = create_box("box"+current_iterator);
        move_to(box, MegaTrash.offsetLeft, MegaTrash.offsetTop);
    }
    current_iterator++;
}

function hide_begin(){
    if(validate() == true){
        $("#exampleModal").modal("hide");
        document.getElementById("Logo").hidden = true;
        document.getElementById("start").hidden = true;
        document.getElementById("day_info").hidden = false;
        document.getElementById("next_day").hidden = false;
        position();
        $.get(
            "http://localhost:3422",
            {market : value_markets, day : value_days, product : value_product, percentup : percent_up, percentdown : percent_down},
            function(data){
                console.log(data);
                console.log("function(data) work!");
                succes_data = data;
                interval_id =  setInterval(parse, 100);
            }
        );
    }
}

function validate() {
    var market = Number(document.getElementById("value_market").value);
    console.log(market);
    var days = Number(document.getElementById("value_days").value);
    var product = Number(document.getElementById("value_product").value);

    for(var i = 0; i<product; ++i){
        current_product.push(product_list[i]);
        current_null.push(0);
        current_null_bad.push(0);
    }
    console.log(current_product);
    var p_up = Number(document.getElementById("percent_up").value);
    var p_down = Number(document.getElementById("percent_down").value);
    var alert_text = "";
    document.getElementById("danger").innerHTML = "";
    if(days < 1 ||days > 30 ){
        alert_text = "Количество дней должно находиться в диапазоне от 1 до 30 включительно!";
        document.getElementById("danger").innerHTML += alert_text;
        document.getElementById("danger").hidden = false;
        return false;
    }
    if(market < 3 || market > 9){
        alert_text = "Количество заказчиков должно находиться в диапазоне от 3 до 9 включительно!";
        document.getElementById("danger").innerHTML += alert_text;
        document.getElementById("danger").hidden = false;
        return false;
    }
    if (product <= 0){
        alert_text = "Количество продуктов должно быть целым положительным числом";
        document.getElementById("danger").innerHTML += alert_text;
        document.getElementById("danger").hidden = false;
        return false;
    }
    if (p_up < 0 || p_down < 0){
        alert_text = "Число процентов должно быть неотрицательным числом";
        document.getElementById("danger").innerHTML += alert_text;
        document.getElementById("danger").hidden = false;
        return false;
    }  
    else{
        value_markets = market;
        value_days = days;
        value_product = product;
        percent_up = p_up;
        percent_down = p_down;
    }
    return true;
}


function img_click(market_id) {
    $("#MarketInfoModal").modal("show");    
    var inf = document.getElementById("info_market");
    var label = document.getElementById("MarketInfoModalLabel");
    label.innerHTML = "Информация о заказчике " + (market_id+1);
    inf.innerHTML = '<canvas id="myCanvas'+market_id+'" width="480" height="320"></canvas>';
    const ctx = document.getElementById('myCanvas'+market_id).getContext("2d");
    const myChart = new Chart(ctx, {
        type: 'bar',
        data: {
            labels: ['Заказов сделано', 'Из них выполнено'],
            datasets: [{
                label: '#Количество',
                data: [markets[market_id].all_orders, markets[market_id].complete_orders],
                backgroundColor: [
                    'rgba(255, 99, 132, 0.2)',
                    'rgba(54, 162, 235, 0.2)',
                ],
                borderColor: [
                    'rgba(255, 99, 132, 1)',
                    'rgba(54, 162, 235, 1)',
                ],
                borderWidth: 1
            }]
        },
        options: {
            scales: {
                y: {
                    beginAtZero: true
                }
            }
        }
    });
}

function create_box(seed) {
    var box = new Image();
    box.src = "https://i.ibb.co/nR3JSxN/box.png";
    box.style = 
    `position: absolute;
     width: 100px;
     height: 100px;
     margin-left:45%;
     margin-top:25%;`;
    box.id = "box"+seed;
    document.body.appendChild(box);
    return box;
}

function create_order(seed, where){
    var order = new Image();
    order.src = "https://i.ibb.co/rtXBJZT/order.png";
    order.style =
        `position: absolute;
        width: 100px;
        height: 100px;
        margin-left:${where.offsetLeft}px;
        margin-top:${where.offsetTop}px;`;
    order.id = "order"+seed;
    document.body.appendChild(order);
    return order;
}

function move_to(object, x, y, order_box = "box"){
    console.log("go to " + x + " "+ y );
    var deltax = object.offsetLeft - x;
    var deltay = object.offsetTop - y;
    var time_go = 500;
    if(order_box == "order"){
        $("#"+object.id).animate({marginTop : "-=" + deltay + "px"},time_go);
        $("#"+object.id).animate({marginLeft : "-=" + deltax + "px"},time_go, 
        function(){$("#"+object.id).remove()});
    }
    else{
        $("#"+object.id).animate({marginLeft : "-=" + deltax + "px"},time_go);
        $("#"+object.id).animate({marginTop : "-=" + deltay + "px"},time_go, 
        function(){$("#"+object.id).remove()});
    }
} 
