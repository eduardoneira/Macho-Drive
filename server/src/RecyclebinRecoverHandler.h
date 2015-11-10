#ifndef RECYCLEBINRECOVERHANDLER_H
#define RECYCLEBINRECOVERHANDLER_H

#include "EventHandlerChecksAuthentication.h"

class RecyclebinRecoverHandler : public EventHandlerChecksAuthentication
{
    public:
        RecyclebinRecoverHandler(Database* db, TokenAuthenticator *a);
        virtual ~RecyclebinRecoverHandler();
    protected:
        void _handle(HttpRequest &hmsg);
    private:
};

#endif // RECYCLEBINRECOVERHANDLER_H
