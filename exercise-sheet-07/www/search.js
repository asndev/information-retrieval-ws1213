var port = 8888;
var setMatches;
var searchWord = "";
var start_time;
var queryValue;
var contentValue;

$(document).ready(function(){
  $("#query").autocomplete({
    source: processQuery,
    select: processContent
  });
  $("#queryValue").change(onQueryValueChange);
  $("#contentValue").change(onContentValueChange);
  queryValue = 10;
  contentValue = 10;
  var cookies = document.cookie.split(";");
  for (var i = 0; i < cookies.length; i++) {
    var args = cookies[i].replace(/\s/g, "").split("=");
    if (args[0] == "qValue") {
      $("#queryValue").val(args[1]);
      queryValue = args[1];
    }
    if (args[0] == "cValue"){
      $("#contentValue").val(args[1]);
      contentValue = args[1];
    }
  }
  $(window).scroll(function () { refreshCookieExpire(); });
});

// Changeevents for option fields.
function onQueryValueChange(eventObj) {
  var qv = $("#queryValue").val();
  if (!isNaN(qv) && qv > 0) {
    queryValue = qv;
    refreshCookieExpire();
  } else {
    $("#queryValue").val(queryValue);
  }
}

function onContentValueChange(eventObj) {
  var cv = $("#contentValue").val();
  if (!isNaN(cv) && cv > 0) {
    contentValue = cv;
    refreshCookieExpire
  } else {
    $("#contentValue").val(contentValue);
  }
}

function refreshCookieExpire() {
  var exp = new Date();
    exp.setMinutes(exp.getMinutes() + 1);
    document.cookie = "qValue=" + queryValue + "; expires=" + exp;
    document.cookie = "cValue=" + contentValue + "; expires=" + exp;
}

// Process autocompletion query.
function processQuery(query, setMatchesFunction) {
  setMatches = setMatchesFunction;
  //console.log("Query: " + query.term);
  $.ajax({
    url: "http://localhost:" + port,
    data: {
      "query" : query.term,
      "numSuggestions" : queryValue,
      "numHits" : contentValue
    },
    dataType: "jsonp"
  });
}

function processContent(evt, ui) {
  contentQuery(ui.item.value);
}

// Process result of autocompletion query.
function queryCallback(json) {
  var matches = $.map(json.matches, function(match) {
      return { label: match, value: match };
  });
  setMatches(matches);
  // Show top result while user is typing.
  contentQuery(json.matches[0]);
}

function contentQuery(needle) {
  start_time = new Date().getTime();
  searchWord = needle;
    $.ajax({
      url: "http://localhost:" + port,
      data: {
        "content" : needle,
        "numSuggestions" : queryValue,
        "numHits" : contentValue
      },
      dataType: "jsonp"
    })
}

function contentCallback(json) {
  var timings = new Date().getTime() - start_time;
  $(".results").empty();
  for (var entry in json.content) {
    var obj = json.content[entry];
    var data = "<div class='res'>" +
      "<p class='title'>" +
        "<a href='" + obj.title + "' target='_blank'>" +
        obj.title.substring(obj.title.search("wiki/") + 5).replace(new RegExp("_","g")," ") +
        "</a>" +
      "</p>" +
      "<p class='link'>" +
      obj.title + // "<font color='#666'> teilen</font>" +
      "</p>" +
      "<div class='record'>" +
      obj.record +
      "</div>" +
      "</div>";
    //var hidden = $(data).hide();
    $(".results").append(data);
    //hidden.show();
  }

  $(".approx").empty();
  $(".approx").append("showing  " + Math.min(contentValue, json.count) +
                      " (of "+ json.count + ") results [" + timings + " ms]");
  $(".record").highlight(searchWord);
}

function eraseCallback() {
  $(".results").empty();
  var data = "<div class='res'>" +
      "<p class='title'>" +
        "nothing found." +
        "</a>" +
      "</p>" +
      "</div>";
  $(".results").append(data);
}