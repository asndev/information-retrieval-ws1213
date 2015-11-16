// Copyright 2012 Anton Stepan <stepana@informatik.uni-freiburg.de>

#include "./SearchServer.h"
#include <boost/asio.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <cassert>
#include <string>
#include <vector>
#include "./Posting.h"

using boost::asio::ip::tcp;
using std::cout;
using std::endl;
using std::flush;
using std::cerr;

// Top10 results
const size_t overallResults = 10;

// _____________________________________________________________________________
SearchServer::SearchServer(const ApproximateMatching& am,
                           const InvertedIndex& ii,
                           const size_t port) {
  _am = am;
  _ii = ii;
  _port = port;
}

// _____________________________________________________________________________
void SearchServer::ServerLoop() {
  // Now the server part.
  try {
    // Create socket and bind to and listen on given port.
    boost::asio::io_service io_service;
    tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), _port));
    // Wait for requests and process them.
    size_t i = 0;
    while (true) {
      std::vector<const std::string*> vocabulary;
      std::ostringstream answer;

      // Wait for request.
      cout << "\x1b[1m\x1b[34m[" << (++i) << "] Waiting for query on port "
           << _port << " ... \x1b[0m" << flush;
      tcp::socket socket(io_service);
      acceptor.accept(socket);
      std::time_t now = std::time(0);
      std::string daytime = std::ctime(&now);  // NOLINT
      cout << "done, received new request on " << daytime << std::flush;

      // Get the request string.
      std::vector<char> requestBuffer(1000);
      boost::system::error_code read_error;
      socket.read_some(boost::asio::buffer(requestBuffer), read_error);
      std::string request(requestBuffer.size(), 0);
      std::copy(requestBuffer.begin(), requestBuffer.end(), request.begin());
      for (size_t i = 0; i < request.size(); ++i)
        request[i] = isspace(request[i]) ? ' ' : request[i];
      cout << endl << "request string is \""
           << (request.size() < 99 ? request : request.substr(0, 87) + "...")
           << "\"" << endl << endl;
      // Extract query string.
      std::string query;
      size_t pos1 = request.find("query=");
      size_t pos2 = request.find("&", pos1);
      if (pos1 != std::string::npos || pos2 != std::string::npos) {
        query = request.substr(pos1 + 6, pos2 - pos1 - 6);
        boost::algorithm::replace_all(query, "+", " ");
      }
      cout << "query string is \"" << query << "\"" << endl;
      if (query.size() > 0) {
        // Find all words in vocabulary starting with query string.
        std::ostringstream matches;
        size_t numMatches = 0;
        size_t maxEditSystem = std::floor((query.size() - 1)/3);
        vocabulary = _am.ComputeApproximateMatches(query, maxEditSystem);

        for (size_t i = 0; i < vocabulary.size(); ++i) {
          matches << "\"" << (*vocabulary[i]) << "\", ";
          if (++numMatches >= 20) break;
        }

        // Send a JSONP object containing the answer.
        std::ostringstream jsonp;
        jsonp << "queryCallback({\n"
              << "  matches: [ " << matches.str() << " ]\n"
              << "})";
        answer << "HTTP/1.0 200 OK\r\n"
               << "Content-Length: " << jsonp.str().size() << "\r\n"
               << "Content-Type: application/javascript" << "\r\n"
               << "Connection: close\r\n"
               << "\r\n"
               << jsonp.str();
      }

      // Extract contant string
      std::string content;
      pos1 = request.find("content=");
      pos2 = request.find("&", pos1);
      if (pos1 != std::string::npos && pos2 != std::string::npos) {
        content = request.substr(pos1 + 8, pos2 - pos1 - 8);
        boost::algorithm::replace_all(content, "+", " ");
      }
      cout << "contant string is \"" << content << "\"" << endl;

      if (content.size() > 0) {
        std::vector<Posting> matchings = _ii.getPostingForWord(content);
        // Sorting matchings regarding their scores.
        size_t partialNumber = std::min(matchings.size(), overallResults);
        std::partial_sort(matchings.begin(),
          matchings.begin() + partialNumber,
          matchings.end(),
          [](const Posting& a, const Posting& b) {
            return a.score > b.score;
          });
        // Getting output json object with title and record for each matching.
        std::ostringstream resultContent;
        size_t count = 1;
        for (auto it = matchings.begin(); it != matchings.end(); ++it) {
          int id = it->docId - 1;
          string title = _ii.getUrl(id);
          string record = _ii.getRecord(id);
          boost::algorithm::replace_all(record, "\"", "\\\"");
          boost::algorithm::replace_all(record, "'", "\\'");
          resultContent << "{"
          << "\"title\": \"" << title << "\", "
          << " \"record\": \"" << record << "\""
          << "}, ";
          ++count;
          if (count > overallResults) break;
        }
        // Send a JSONP object containing the answer.
        std::ostringstream jsonp;
        jsonp << "contentCallback({\n"
              << "  content: [ " << resultContent.str() << " ],\n"
              << "  count: " << matchings.size() << "\n"
              << "})";

        answer << "HTTP/1.0 200 OK\r\n"
               << "Content-Length: " << jsonp.str().size() << "\r\n"
               << "Content-Type: application/javascript" << "\r\n"
               << "Connection: close\r\n"
               << "\r\n"
               << jsonp.str();
      }

      // Prevents error for unknown queryword.
      if (query == "" && content == "") {
        std::ostringstream jsonp;
        jsonp << "eraseCallback({})";
        answer << "HTTP/1.0 200 OK\r\n"
               << "Content-Length: " << jsonp.str().size() << "\r\n"
               << "Content-Type: application/javascript" << "\r\n"
               << "Connection: close\r\n"
               << "\r\n"
               << jsonp.str();
      }

      boost::system::error_code write_error;
      boost::asio::write(socket, boost::asio::buffer(answer.str()),
          boost::asio::transfer_all(), write_error);
    }
  } catch(const std::exception& e) {  // NOLINT.
    cerr << e.what() << endl;
  }
}
