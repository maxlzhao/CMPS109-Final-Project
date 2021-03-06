#include "HTTPServiceManager.h"
#include "HTTPNotAcceptableExceptionHandler.h"
#define WEB_CACHE_ROOT  "/Users/maxzhao/Desktop/CMPS109/takeHomeMidterm/www"
// Constructor:  building up the factory map
HTTPServiceManager::HTTPServiceManager()
{
    // adding the html and form service cloners
    services ["html"] = new HTTPHTMLService(new FileCache(WEB_CACHE_ROOT),true);
    services ["form"] = new HTTPXMLService(); // upon form, enter new XML service
    services ["png"] = new HTTPImageService(new FileCache(WEB_CACHE_ROOT),true); // adds support for png
    services ["jpg"] = new HTTPImageService(new FileCache(WEB_CACHE_ROOT),true); // adds support for jpg
    services ["gif"] = new HTTPImageService(new FileCache(WEB_CACHE_ROOT),true); // adds support for gif
}
// Compare the file extention to the map key first and if not found compare the whole file name
HTTPService * HTTPServiceManager::getService (string p_resource)
{
    // extract extentions
    string ext = p_resource.substr(p_resource.find_last_of(".") + 1);
    if ( services[ext]==NULL)  // if not found
    {
        // Extract file base noame
        string base_name = p_resource.substr(p_resource.find_last_of("/") + 1);
        // If not found also throw and exception
        if ( services[base_name]==NULL) throw (HTTPNotAcceptableExceptionHandler());
        else return services[base_name]->clone(); // else clone service based on base file name
    }
    else return services[ext]->clone(); // clone service based on extension
}
// Destructor
HTTPServiceManager::~HTTPServiceManager()
{
    // A for_each iterator based loop with lambda function to deallocate all the cloner objects

    for_each (services.begin(),services.end(),[](const std::pair<string,HTTPService *>& it) -> bool {
        HTTPService * httpService = std::get<1>(it);
        delete(httpService);
        return true;
   });
}
