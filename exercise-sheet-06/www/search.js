var port = 8888;
var setMatches;
var searchWord = "";
var start_time;

$(document).ready(function(){
  $("#query").autocomplete({
    source: processQuery,
    select: processContent
  });
});

// Process autocompletion query.
function processQuery(query, setMatchesFunction) {
  setMatches = setMatchesFunction;
  //console.log("Query: " + query.term);
  $.ajax({
    url: "http://localhost:" + port,
    data: {
      "query" : query.term
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
        "content" : needle
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
  $(".approx").append("overall results: " + json.count + " (" + timings + " ms)");
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