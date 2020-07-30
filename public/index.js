// Put all onload AJAX calls here, and event listeners
$(document).ready(function() {

    // On page-load AJAX code
    $("#attrTextArea").val(' ');
    $("#tbody-gpsview").empty();
    $('#selectGPX').empty().append("<option disabled selected>" + "Select GPX File" + "</option>");
    $('#selectAdd').empty().append("<option disabled selected>" + "Select GPX File" + "</option>");
    $('#selectFile').empty().append("<option disabled selected>" + "Select GPX File" + "</option>");
    $('#selectFile2').empty().append("<option disabled selected>" + "Select GPX File" + "</option>");
    $('#selectFile3').empty().append("<option disabled selected>" + "Select GPX File" + "</option>");
    $('#selectQuery').prop('selectedIndex',0);
    $('#selectSort1').prop('selectedIndex',0);
    $('#selectSort2').prop('selectedIndex',0);
    $('#selectRouteTrack').empty().append("<option disabled selected>" + "Select Route/Track" + "</option>");
    $('#selectRoute').empty().append("<option disabled selected>" + "Select Route" + "</option>");
    $('#enterName').empty();
    $('#enterRName').val("");
    if ($('#submitFile').is(":disabled")) {
            $('#submitFile').css("background-color", "#436778");
    }
    $('#enterName').val(" ");
    $("#enterLongi1").val("");
    $("#enterLati1").val("");
    $("#enterLongi2").val("");
    $("#enterLati2").val("");
    $("#enterDelta").val("");
    $('#selectQuery').hide();
    $('#queryBtn').hide();
    $("#enterDBName").val("");
    $("#enterUser").val("");
    $("#enterPass").val("");
    //Database on load
    var login = false;
    var empty = false;

  var waypointList = []
  //Get the file names and place it into Dropdown menu
  var fileNames = [];
    $.ajax({
        type: 'get',            //Request type
        dataType: 'json',       //Data type - we will use JSON for almost everything
        url: '/getFileNames',   //The server endpoint we are connecting to
        success: function (data) {
          console.log(data);
          fileNames = data;
          if (fileNames.length > 0) {
            $("#status").remove();
          }
          else {
              $('#storeBtn').remove();
          }
          for (x = 0; x < fileNames.length; x++) {
            $("#selectGPX").append("<option>" + fileNames[x] + "</option>");
            $("#selectAdd").append("<option>" + fileNames[x] + "</option>");
            $("#selectFile").append("<option>" + fileNames[x] + "</option>");
            $("#selectFile2").append("<option>" + fileNames[x] + "</option>");
            $("#selectFile3").append("<option>" + fileNames[x] + "</option>");
          }
            //We write the object to the console to show that the request was successful
        },
        fail: function(error) {
            // Non-200 return, do something with error
            console.log(error);
        }
    });


    //Display information of files in uploads into the File Log;
    $.ajax({
      type: 'get',
      dataType: 'json',
      url: '/makeFileLog',
      success: function (data) {
        console.log(data);
        var fileLogList = data;
        for (var x = 0; x < fileLogList.length; x++) {
          $("#tbody-filelog").append("<tr>" +
          "<td>" + "<a href=\"/uploads/" + fileLogList[x].name + "\">" + fileLogList[x].name +"</a>" + "</td>" +
          "<td>" + fileLogList[x].version + "</td>" +
          "<td>" + fileLogList[x].creator + "</td>" +
          "<td>" + fileLogList[x].numWaypoints + "</td>" +
          "<td>" + fileLogList[x].numRoutes + "</td>" +
          "<td>" + fileLogList[x].numTracks + "</td>" +
          "</tr>");
        }
      },
      fail: function (error) {
        console.log(error);
      }
    });

    //Show attributes javascript
    $("#showAttr").on('click', function() {
        var currSelect = $("#selectGPX").find(":selected").text();
        var attrSelect = $("#selectRouteTrack").find(":selected").text();
        if (attrSelect == "Select Route/Track") {
          return false;
        }
        $.ajax({
          type: 'get',
          data: {currSelect:currSelect, attrSelect:attrSelect},
          dataType: 'json',
          url: '/getAttributes',
          success: function (data) {
            var attrList = data;
              for (x = 0; x < attrList.length; x++) {
                var attr = attrList[x];
                if (attrList[x].length == 0) {
                  alert("No attributes.");
                }
                else {
                  var display = "Attributes Listed";
                  for (var y = 0; y < attrList.length; y++) {
                    display += ("\n\nName: " + attr[y].name + "\nValue: " + attr[y].value);
                  }
                  alert(display);
                }
            }
          },
          fail: function (error) {
            alert("Show attributes failed. File may be corrupt.");
          }
    });
  });

    //Upload GPX
    $("#selectGPX").change(function updateGPXPanel () {
        var currSelect = $("#selectGPX").find(":selected").text();
        $.ajax({
          type: 'get',
          data: {currSelect:currSelect},
          dataType: 'json',
          url: '/makeGPSPanel',
          success: function (data) {
            var routeTracksList = data;
            var y = 1;
            var routesList = routeTracksList[0];
            var tracksList = routeTracksList[1];
            $("#tbody-gpsview").empty();
            $('#selectRouteTrack').empty().append("<option disabled selected>" + "Select Route/Track" + "</option>");

            //Print out routes
            for (var x = 0; x < routesList.length; x++) {
              $("#tbody-gpsview").append("<tr>" +
              "<td>" + "Route " + y + "</td>" +
              "<td>" + routesList[x].name + "</td>" +
              "<td>" + routesList[x].numPoints + "</td>" +
              "<td>" + routesList[x].len + "</td>" +
              "<td>" + routesList[x].loop + "</td>" +
              "</tr>");
              $('#selectRouteTrack').append("<option>" + "Route " + y + "</option>");
              y++;
            }
            //Print out tracks
            y = 1;
            for (var x = 0; x < tracksList.length; x++) {
              $("#tbody-gpsview").append("<tr>" +
              "<td>" + "Track " + y + "</td>" +
              "<td>" + tracksList[x].name + "</td>" +
              "<td>" + "</td>" +
              "<td>" + tracksList[x].len + "</td>" +
              "<td>" + tracksList[x].loop + "</td>" +
              "</tr>");
              $('#selectRouteTrack').append("<option>" + "Track " + y + "</option>")
              y++;
            }
        },
        fail: function (error) {
            console.log(error);
        }
      });
  });

    // File type validation
    $("#chooseFile").change(function() {
        var file = this.files[0];
        var fileType = file.name.split('.').pop();
        if(!(fileType == "gpx")) {
            alert('Sorry, only .gpx files are allowed to be uploaded.');
            $("#submitFile").prop("disabled", true);
            $('#submitFile').css("background-color", "#436778");
            return false;
        }
        else {
            for (var x = 0; x < fileNames.length; x++) {
                if (fileNames[x] == file.name) {
                  alert('This file already exists. Choose another');
                  $("#submitFile").prop("disabled", true);
                  $('#submitFile').css("background-color", "#436778");
                  return false;
                }
            }

            $("#submitFile").prop("disabled", false);
            $('#submitFile').css("background-color", "#044552");
        }
    });

    //Create GPX form open
    $("#create").click(function () {
      $("#createForm").css("display", "block");
    });
    //Create GPX form close
    $("#closeGPX").click(function (e) {
      e.preventDefault();
      $("#createForm").css("display", "none");
    });

    //Create GPX submit form
    $("#createBtn").click(function (e) {
      var fileName = $('#enterFileName').val();
      var creator = $('#enterCreator').val();
      if (!(fileName) || (!(creator))) {
        alert('Please fill out all sections.');
        return false;
      }

      if (fileName.split('.').pop() != "gpx") {
        alert("Please enter a .gpx extention for the file name.");
        return false;
      }

            $.ajax({
                 dataType: 'json',
                 type: 'get',
                 url: '/createGPX',
                 data: {
                         fileName: fileName,
                         creator: creator
                       },
                 success: function (data) {
                   var result = data;
                   if (result == true) {
                       alert("Success! " + fileName + " was created.");
                    }
                    else if(result == false){
                         alert("Invalid GPX or File Name is already inuse. Creation of " + fileName + " was unsuccessful.");
                    }

                 },
                 fail: function (error) {
                   console.log(error);
                 }
            });

      $('#enterFileName').val("");
      $('#enterVersion').val("");
      $('#enterCreator').val("");

      //Display information of files in uploads into the File Log;
      $('#tbody-filelog').empty();
      $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/makeFileLog',
        success: function (data) {
          console.log(data);
          $('#selectGPX').empty().append("<option disabled selected>" + "Select GPX File" + "</option>");
          $('#selectAdd').empty().append("<option disabled selected>" + "Select GPX File" + "</option>");
          var fileLogList = data;
          for (var x = 0; x < fileLogList.length; x++) {
            $("#tbody-filelog").append("<tr>" +
            "<td>" + "<a href=\"/uploads/" + fileLogList[x].name + "\">" + fileLogList[x].name +"</a>" + "</td>" +
            "<td>" + fileLogList[x].version + "</td>" +
            "<td>" + fileLogList[x].creator + "</td>" +
            "<td>" + fileLogList[x].numWaypoints + "</td>" +
            "<td>" + fileLogList[x].numRoutes + "</td>" +
            "<td>" + fileLogList[x].numTracks + "</td>" +
            "</tr>");
            $("#selectAdd").append("<option>" + fileLogList[x].name + "</option>");
            $("#selectGPX").append("<option>" + fileLogList[x].name + "</option>");
          }
        },
        fail: function (error) {
          console.log(error);
        }
      });
  });

  //Rename form open
  $("#rename").click(function () {
      $("#renameForm").css("display", "block");
    });

  //Rename form close
  $("#closeRename").click(function (e) {
      e.preventDefault();
      $("#renameForm").css("display", "none");
    });

  //Rename submit name
  $('#renameBtn').click(function (e) {

      var newname = $('#enterName').val();
      var currSelect = $("#selectGPX").find(":selected").text();
      var attrSelect = $("#selectRouteTrack").find(":selected").text();
      if (currSelect == "Select GPX File") {
        alert('Please select a GPX file from the drop menu beside GPX View Panel');
        return false;
      }
      if (attrSelect == "Select Route/Track") {
        alert('Please select a Route/Track drop menu beside Rename');
        return false;
      }
      if (newname == " " || !(newname)) {
        alert('Please enter a name or close.');
        return false;
      }
      //Make call to rename
       $.ajax({
          dataType: 'json',
          type: 'get',
          url: '/rename',
          data: {
                  currSelect: currSelect,
                  attrSelect: attrSelect,
                  newname: newname
                },
          success: function (data) {
            var result = data;
            if (result == 1) {
                alert("Success! " + currSelect + ":" + attrSelect + " was renamed.");
              }
              else {
                alert("Something went wrong. Renaming of " + currSelect + ":" + attrSelect + "was unsuccessful.");
              }
          },
          fail: function (error) {
            console.log(error);
          }
      });

      $('#enterName').val(" ");

      //Update the table
      var currSelect = $("#selectGPX").find(":selected").text();
      $.ajax({
        type: 'get',
        data: {currSelect:currSelect},
        dataType: 'json',
        url: '/makeGPSPanel',
        success: function (data) {
          var routeTracksList = data;
          var y = 1;
          var routesList = routeTracksList[0];
          var tracksList = routeTracksList[1];
          $("#tbody-gpsview").empty();
          $('#selectRouteTrack').empty().append("<option disabled selected>" + "Select Route/Track" + "</option>");

          //Print out routes
          for (var x = 0; x < routesList.length; x++) {
            $("#tbody-gpsview").append("<tr>" +
            "<td>" + "Route " + y + "</td>" +
            "<td>" + routesList[x].name + "</td>" +
            "<td>" + routesList[x].numPoints + "</td>" +
            "<td>" + routesList[x].len + "</td>" +
            "<td>" + routesList[x].loop + "</td>" +
            "</tr>");
            $('#selectRouteTrack').append("<option>" + "Route " + y + "</option>");
            y++;
          }
          //Print out tracks
          y = 1;
          for (var x = 0; x < tracksList.length; x++) {
            $("#tbody-gpsview").append("<tr>" +
            "<td>" + "Track " + y + "</td>" +
            "<td>" + tracksList[x].name + "</td>" +
            "<td>" + "</td>" +
            "<td>" + tracksList[x].len + "</td>" +
            "<td>" + tracksList[x].loop + "</td>" +
            "</tr>");
            $('#selectRouteTrack').append("<option>" + "Track " + y + "</option>")
            y++;
          }
      },
      fail: function (error) {
          console.log(error);
      }
    });
  });

  //Add route form open
  $("#addRoute").click(function () {
    $("#addRForm").css("display", "block");
  });

  //Add route form close
  $("#closeR").click(function (e) {
    e.preventDefault();
    $("#addRForm").css("display", "none");
    waypointList.length = 0;
  });

  //Add a waypoint to a route
  $("#wayBtn").click(function (e) {
      var latitude = $('#enterLat').val();
      var longitude = $('#enterLong').val();


      if ((!(latitude)) || (!(longitude))) {
        alert('Please enter both longitude and latitude.');
        return false;
      }
      if (isNaN(longitude)){
        alert('Longitude is not a number.');
        return false;
      }
      if (isNaN(latitude)){
        alert('Latitude is not a number.');
        return false;
      }
      if ((latitude > 85) || (latitude < -85)) {
        alert('Latitude out of range (-85 to +85 degrees)');
        return false;
      }
      if ((longitude > 180) || (longitude < -180)) {
        alert('Longitude out of range (-180 to +180 degrees)');
        return false;
      }


      latitude = parseFloat(latitude);
      longitude = parseFloat(longitude);

      var waypoint = JSON.stringify({lat: latitude, lon: longitude});
      waypointList.push(waypoint);

      $('#enterLat').val("");
      $('#enterLong').val("");
  });

  //Add a route to a gpx file
  $("#routeBtn").click(function (e) {
    var fileName = $("#selectAdd").find(":selected").text();
    var routeName = $("#enterRName").val();

    if (!(routeName)) {
      alert('Please enter a route name.');
      return false;
    }
    if (fileName == 'Select GPX File') {
      alert('Please select a file from the drop down menu.');
      return false;
    }

    routeName = JSON.stringify({name: routeName});

    $.ajax({
      url: '/addRoute',
      type: 'get',
      dataType: 'json',
      data: {
              routeName: routeName,
              fileName: fileName,
              waypointList: waypointList
      },
      success: function (data) {
            var result = data;
            if (result == true) {
              alert('Success! Route was added.');
            }
            else if (result == false) {
              alert('Sorry, something went wrong. Route was not added');
            }
      },
      fail: function (error) {
        console.log(error);
      }
    });

    //Display information of files in uploads into the File Log;
    $('#tbody-filelog').empty();
    $.ajax({
      type: 'get',
      dataType: 'json',
      url: '/makeFileLog',
      success: function (data) {
        console.log(data);
        $('#selectGPX').empty().append("<option disabled selected>" + "Select GPX File" + "</option>");
        $('#selectAdd').empty().append("<option disabled selected>" + "Select GPX File" + "</option>");
        var fileLogList = data;
        for (var x = 0; x < fileLogList.length; x++) {
          $("#tbody-filelog").append("<tr>" +
          "<td>" + "<a href=\"/uploads/" + fileLogList[x].name + "\">" + fileLogList[x].name +"</a>" + "</td>" +
          "<td>" + fileLogList[x].version + "</td>" +
          "<td>" + fileLogList[x].creator + "</td>" +
          "<td>" + fileLogList[x].numWaypoints + "</td>" +
          "<td>" + fileLogList[x].numRoutes + "</td>" +
          "<td>" + fileLogList[x].numTracks + "</td>" +
          "</tr>");
          $("#selectAdd").append("<option>" + fileLogList[x].name + "</option>");
          $("#selectGPX").append("<option>" + fileLogList[x].name + "</option>");
        }
      },
      fail: function (error) {
        console.log(error);
      }
    });
    $('#enterRName').val("");
    waypointList.length = 0;
  });

  //Find the paths between two points
  $("#findBtn").click(function (e) {
      var long1 = $("#enterLongi1").val();
      var lat1 = $("#enterLati1").val();
      var long2 = $("#enterLongi2").val();
      var lat2 = $("#enterLati2").val();
      var delta = $("#enterDelta").val();

      if (!(long1) || !(lat1) || !(long2) || !(lat2) || !(delta) ) {
        alert('Please completely fill longitudes, latitudes and delta.');
      }
      if (isNaN(long1)){
        alert('Start longitude is not a number.');
        return false;
      }
      if (isNaN(lat1)){
        alert('Start latitude is not a number.');
        return false;
      }
      if (isNaN(long2)){
        alert('End longitude is not a number.');
        return false;
      }
      if (isNaN(lat2)){
        alert('End latitude is not a number.');
        return false;
      }
      if (isNaN(delta)){
        alert('Delta is not a number.');
        return false;
      }

      lat1 = parseFloat(lat1);
      long1 = parseFloat(long1);
      lat2 = parseFloat(lat2);
      long2 = parseFloat(long2);
      delta = parseFloat(delta);

      if (delta < 0){
        alert('Delta cannot be negative.');
        return false;
      }
      if ((lat1 > 85) || (lat1 < -85)) {
        alert('Start latitude out of range (-85 to +85 degrees)');
        return false;
      }
      if ((lat2 > 85) || (lat2 < -85)) {
        alert('End latitude out of range (-85 to +85 degrees)');
        return false;
      }
      if ((long1 > 180) || (long1 < -180)) {
        alert('Start longitude out of range (-180 to +180 degrees)');
        return false;
      }
      if ((long2 > 180) || (long2 < -180)) {
        alert('End longitude out of range (-180 to +180 degrees)');
        return false;
      }


      $.ajax({
        url: '/find',
        type: 'get',
        data: {
          startLong:long1,
          startLat:lat1,
          endLong:long2,
          endLat:lat2,
          delta:delta
        },
        dataType: 'json',
        success: function (data){
            $("#pbody-gpsview").empty();
            var foundList = [];
            foundList = data;
            if (foundList[0].length == 0 && foundList[1].length == 0) {
              alert('No routes or tracks found.');
            }
            routesList = foundList[0];
            tracksList = foundList[1];
            var y = 1;
            for (var i = 0; i < routesList.length; i++) {
              var arrayRoutes = routesList[i];
              for (var x = 0; x < arrayRoutes.length; x++) {
                $("#pbody-gpsview").append("<tr>" +
                "<td>" + "Route " + y + "</td>" +
                "<td>" + arrayRoutes[x].name + "</td>" +
                "<td>" + arrayRoutes[x].numPoints + "</td>" +
                "<td>" + arrayRoutes[x].len + "</td>" +
                "<td>" + arrayRoutes[x].loop + "</td>" +
                "</tr>");
                y++;
              }
            }
            //Print out tracks
            y = 1;
            for (var i = 0; i < tracksList.length; i++) {
              var arrayTracks = tracksList[i];
              for (var x = 0; x < arrayTracks.length; x++) {
                $("#pbody-gpsview").append("<tr>" +
                "<td>" + "Track " + y + "</td>" +
                "<td>" + arrayTracks[x].name + "</td>" +
                "<td>" + "</td>" +
                "<td>" + arrayTracks[x].len + "</td>" +
                "<td>" + arrayTracks[x].loop + "</td>" +
                "</tr>");
                y++;
              }
          }
            //Sort the table by length low to high
              var table = $('.lengthFP').parents('table').eq(0);
              var rows = table.find('tr:gt(0)').toArray().sort(comparer($('.lengthFP').index()));
              $('.lengthFP').asc = !$('.lengthFP').asc;
              if (!$('.lengthFP').asc){rows = rows.reverse()}
              for (var i = 0; i < rows.length; i++){table.append(rows[i])}

              $("#enterLongi1").val("");
              $("#enterLati1").val("");
              $("#enterLongi2").val("");
              $("#enterLati2").val("");
              $("#enterDelta").val("");

        }
      });
  });

  //Login form open
  $("#loginBtn").click(function () {
    $("#loginForm").css("display", "block");
  });

  //Login form close
  $("#closeLogin").click(function (e) {
    e.preventDefault();
    $("#loginForm").css("display", "none");
  });

  //Login to the dataabase system
  $("#logBtn").click(function (e) {
    var database = $("#enterDBName").val();
    var username = $("#enterUser").val();
    var password = $("#enterPass").val();
    var result = false;

    if (!(database)) {
      alert("Please enter database name.");
      return false;
    }
    if (!(username)) {
      alert("Please enter username.");
      return false;
    }
    if(!(password)) {
      alert("Please enter password.");
      return false;
    }

    $.ajax({
      type: 'get',
      dataType: 'json',
      url: '/login',
      data: {
          username: username,
          password: password,
          database: database
      },
      success: function (data) {
        result = data;
        if (result == true) {
          alert("Login Successful.");
          login = true;
          $('#logStatus').text("Status: logged ON");
          $("#loginForm").css("display", "none");
        }
        else {
          alert("Invalid Login. Re-enter credentials.");
          login = false;
        }
      }
    });

    $("#enterDBName").val("");
    $("#enterUser").val("");
    $("#enterPass").val("");
  });



  $('#storeBtn').click(function(e) {
    if (login == false) {
      alert('Not connected to database. Please log into the database.');
      return false;
    }

    $.ajax({
      type: 'get',
      dataType: 'json',
      url: '/storeFiles',
      success: function (data) {
        console.log("Files have been stored.");
        var array = [];
        array = data;
        alert("Database has " + array[0] + " files, " + array[1] + " routes, and " + array[2] + " points.");
        $('#selectQuery').show();
        $('#queryBtn').show();
      },
      fail: function (error) {
        console.log("Store Files Error: " + error);
      }
    });

    e.preventDefault();
  });

  $('#clearBtn').click(function(e) {
    if (login == false) {
      alert('Not connected to database. Please log into the database.');
      return false;
    }

    $.ajax({
      type: 'get',
      dataType: 'json',
      url: '/clearData',
      success: function (data) {
        console.log("Files have been cleared.");
        var array = [];
        array = data;
        alert("Database has " + array[0] + " files, " + array[1] + " routes, and " + array[2] + " points.");
        $("#bodyDB").empty();
        $("#theadDB").empty().append("<tr><th class=\"tableDB\">Table</th></tr>");
        $('#selectQuery').hide();
        $('#queryBtn').hide();
      },
      fail: function (error) {
        console.log("Clear Data Error: " + error);
      }
    });

    e.preventDefault();
  });

  $('#displayBtn').click(function(e) {
    if (login == false) {
      alert('Not connected to database. Please log into the database.');
      return false;
    }

    callDisplay();

    e.preventDefault();
  });

  $('#queryBtn').click(function(e) {
    var selectedQuery = $('#selectQuery').val();
    if (login == false) {
      alert('Not connected to the database. Please login to use the database.');
      return false;
    }
    if (selectedQuery == null) {
      alert('Please select a query');
      return false;
    }

    if (selectedQuery == "Display all routes") {
      $("#query1Form").css("display", "block");
    }
    else if (selectedQuery == "Display the routes from a specific file") {
      $("#query2Form").css("display", "block");
    }
    else if (selectedQuery == "Display all points of a specific route") {
      $("#query3Form").css("display", "block");
    }
    else if (selectedQuery == "Display all points from a specific file") {
      $("#query4Form").css("display", "block");
    }
    e.preventDefault();
  });

  $('#executeBtn1').click(function(e) {
    var selectedQuery = $('#selectQuery').val();
    if (selectedQuery == "Display all routes") {
      var sorting = $('#selectSort1').val();
      if (sorting == null) {
        alert('Please select a sorting method.');
        return false;
      }
      else if (sorting == 'Name') {
        var input = "SELECT route_id, route_len, route_name, file_name FROM ROUTE, FILE WHERE ROUTE.gpx_id=FILE.gpx_id ORDER BY route_name;";
        displayRoutes(input, 1);
        $("#query1Form").css("display", "none");
      }
      else if (sorting == 'Length') {
        var input = "SELECT route_id, route_len, route_name, file_name FROM ROUTE, FILE WHERE ROUTE.gpx_id=FILE.gpx_id ORDER BY route_len;";
        displayRoutes(input, 2);
        $("#query1Form").css("display", "none");
      }
    }
    e.preventDefault();
  });

  $('#executeBtn2').click(function(e) {
    var selectedQuery = $('#selectQuery').val();
    if (selectedQuery == "Display the routes from a specific file") {
      var sorting = $('#selectSort2').val();
      if (sorting == null) {
        alert('Please select a sorting method.');
        return false;
      }
      var file = $('#selectFile').val();
      if (file == null) {
        alert('Please select a file');
        return false;
      }
      if (sorting == 'Name') {
        var input = "SELECT route_id, route_len, route_name, file_name FROM ROUTE, FILE WHERE ROUTE.gpx_id=FILE.gpx_id AND ROUTE.gpx_id=(SELECT gpx_id FROM FILE WHERE file_name='" + file + "') ORDER BY route_name;";
        displayRoutes(input, 1);
        $("#query2Form").css("display", "none");
      }
      else if (sorting == 'Length') {
        var input = "SELECT route_id, route_len, route_name, file_name FROM ROUTE, FILE WHERE ROUTE.gpx_id=FILE.gpx_id AND ROUTE.gpx_id=(SELECT gpx_id FROM FILE WHERE file_name='" + file + "') ORDER BY route_len;";
        displayRoutes(input, 2);
        $("#query2Form").css("display", "none");
      }
    }
    e.preventDefault();
  });

  $('#executeBtn3').click(function(e) {
    var selectedQuery = $('#selectQuery').val();
    if (selectedQuery == "Display all points of a specific route") {
      var file = $('#selectFile2').val();
      if (file == null) {
        alert('Please select a file');
        return false;
      }
      var route = $('#selectRoute').val();
      if (route == null) {
        alert('Please select a route if possible.');
        return false;
      }
      var input = ("SELECT route_name, file_name, latitude, longitude, point_name, point_index FROM POINT, FILE, ROUTE WHERE POINT.route_id=ROUTE.route_id AND ROUTE.gpx_id=FILE.gpx_id AND POINT.route_id=(SELECT route_id FROM ROUTE WHERE route_name='" + route + "') ORDER BY point_index;");
      displayPoints(input, 1);
      $("#query3Form").css("display", "none");
    }
    e.preventDefault();
  });

  $('#executeBtn4').click(function(e) {
    var selectedQuery = $('#selectQuery').val();
    if (selectedQuery == "Display all points from a specific file") {
      var file = $('#selectFile3').val();
      if (file == null) {
        alert('Please select a file');
        return false;
      }
      var input = ("SELECT route_name, file_name, latitude, longitude, point_name, point_index FROM POINT, FILE, ROUTE WHERE POINT.route_id=ROUTE.route_id AND ROUTE.gpx_id=FILE.gpx_id AND ROUTE.gpx_id=(SELECT gpx_id FROM FILE WHERE file_name='" + file + "') " +
                  "ORDER BY route_name, point_index;");
      displayPoints(input, 2);
      $("#query4Form").css("display", "none");
    }
    e.preventDefault();
  });

  $('#selectFile2').change(function () {
    var file = $('#selectFile2').val();
    $('#selectRoute').empty().append("<option disabled selected>" + "Select Route" + "</option>");
    if (file != 'Select File') {
      $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/routes',
        data: {file:file},
        success: function (data) {
          var name = data;
          if (name.length == 0) {
            $('#selectRoute').empty().append("<option disabled selected>" + "No Routes" + "</option>");
          }
          for (var x = 0; x < name.length; x++) {
            $('#selectRoute').append("<option>" + name[x].name + "</option>");
          }
        }
      });
    }
  });

  $("#closeQuery1").click(function (e) {
    e.preventDefault();
    $("#query1Form").css("display", "none");
  });

  $("#closeQuery2").click(function (e) {
    e.preventDefault();
    $("#query2Form").css("display", "none");
  });

  $("#closeQuery3").click(function (e) {
    e.preventDefault();
    $("#query3Form").css("display", "none");
  });

  $("#closeQuery4").click(function (e) {
    e.preventDefault();
    $("#query4Form").css("display", "none");
  });
  //*********************Functions****************************
  function displayRoutes (input, num) {
    $.ajax({
      type: 'get',
      dataType: 'json',
      url: '/queryInput',
      data: {input:input},
      success: function (data) {
        var result = data;
        if (result.length == 0) {
          alert('No routes were found.');
          return false;
        }
        $("#bodyDB").empty();
        if (num == 1) {
          $("#theadDB").empty().append("<thead><tr><th class=\"routeName\" >Route Name</th>" +
          "<th class=\"routeId\" >Route ID</th>" +
          "<th class=\"routeLen\" >Route Length</th>" +
          "<th class=\"routeGPX\" >File Name</th></tr></thead>");
          for (var x = 0; x < result.length; x++) {
            $("#bodyDB").append(
            "<tr>" +
            "<td class=\"query1td\">" + result[x].route_name + "</td>" +
            "<td class=\"query1td\">" + result[x].route_id + "</td>" +
            "<td class=\"query1td\">" + result[x].route_len + "</td>" +
            "<td class=\"query1td\">" + result[x].file_name + "</td>" +
            "</tr>");
          }
        }
        else if (num == 2){
          $("#theadDB").empty().append("<thead><tr><th class=\"routeLen\" >Route Length</th>" +
          "<th class=\"routeId\" >Route ID</th>" +
          "<th class=\"routeName\" >Route Name</th>" +
          "<th class=\"routeGPX\" >File Name</th></tr></thead>");
          for (var x = 0; x < result.length; x++) {
            $("#bodyDB").append(
            "<tr>" +
            "<td class=\"query1td\">" + result[x].route_len + "</td>" +
            "<td class=\"query1td\">" + result[x].route_id + "</td>" +
            "<td class=\"query1td\">" + result[x].route_name + "</td>" +
            "<td class=\"query1td\">" + result[x].file_name + "</td>" +
            "</tr>");
          }
        }
      }
    });
  }

  function displayPoints (input, num) {
    $.ajax({
      type: 'get',
      dataType: 'json',
      url: '/queryInput',
      data: {input:input},
      success: function (data) {
        var result = data;
        if (result.length == 0) {
          alert('No points were found.');
          return false;
        }
        if (num == 1) {
          $("#bodyDB").empty();
          $("#bodyDB").empty();
          $("#theadDB").empty().append("<thead><tr><th class=\"pointName\" >Point Index</th>" +
          "<th class=\"pointIndex\" >Route Name</th>" +
          "<th class=\"pointLat\" >Latitude</th>" +
          "<th class=\"pointLong\" >Longitude</th>" +
          "<th class=\"pointName\" >Point Name</th>" +
          "<th class=\"routeID\" >File Name</th></tr></thead>");
          var y = 1;
          for (var x = 0; x < result.length; x++) {
            if (result[x].point_name == "") {
              result[x].point_name = "(Empty)";
            }
            if (result[x].route_name == "") {
              result[x].route_name = "Unnamed route " + y;
              y++;
            }
            $("#bodyDB").append(
            "<tr>" +
            "<td class=\"query1td\">" + result[x].point_index + "</td>" +
            "<td class=\"query1td\">" + result[x].route_name + "</td>" +
            "<td class=\"query1td\">" + result[x].latitude + "</td>" +
            "<td class=\"query1td\">" + result[x].longitude + "</td>" +
            "<td class=\"query1td\">" + result[x].point_name + "</td>" +
            "<td class=\"query1td\">" + result[x].file_name + "</td>" +
            "</tr>");
          }
        }
        else if (num == 2) {
          $("#bodyDB").empty();
          $("#theadDB").empty().append("<thead><tr><th class=\"pointName\" >Route Name</th>" +
          "<th class=\"pointIndex\" >Point Index</th>" +
          "<th class=\"pointLat\" >Latitude</th>" +
          "<th class=\"pointLong\" >Longitude</th>" +
          "<th class=\"pointName\" >Point Name</th>" +
          "<th class=\"routeID\" >File Name</th></tr></thead>");
          var y = 1;
          for (var x = 0; x < result.length; x++) {
            if (result[x].point_name == "") {
              result[x].point_name = "(Empty)";
            }
            if (result[x].route_name == "") {
              result[x].route_name = "Unnamed route " + y;
              y++;
            }
            $("#bodyDB").append(
            "<tr>" +
            "<td class=\"query1td\">" + result[x].route_name + "</td>" +
            "<td class=\"query1td\">" + result[x].point_index + "</td>" +
            "<td class=\"query1td\">" + result[x].latitude + "</td>" +
            "<td class=\"query1td\">" + result[x].longitude + "</td>" +
            "<td class=\"query1td\">" + result[x].point_name + "</td>" +
            "<td class=\"query1td\">" + result[x].file_name + "</td>" +
            "</tr>");
          }
        }

      }
    });
  }

  function callDisplay() {
    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/displayDB',
        success: function (data) {
          var array = [];
          array = data;
          alert("Database has " + array[0] + " files, " + array[1] + " routes, and " + array[2] + " points.");
          if (array[0] == 0) {
            $('#selectQuery').hide();
            $('#queryBtn').hide();
          }
          else if (array[0] > 0) {
            $('#selectQuery').show();
            $('#queryBtn').show();
          }

        },
        fail: function (error) {
          alert("Error. Could not display Database Status.");
        }
    });
  }

  //Source: https://stackoverflow.com/questions/3160277/jquery-table-sort
  function comparer(index) {
      return function(a, b) {
          var valA = getCellValue(a, index), valB = getCellValue(b, index);
          return $.isNumeric(valA) && $.isNumeric(valB) ? valB - valA : valB.toString().localeCompare(valA);
      }
  }

  function getCellValue(row, index) {
    return $(row).children('td').eq(index).text()
  }

});
