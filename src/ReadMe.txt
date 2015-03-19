Run Steps:
==========
1. Install the .man file as a provider on the machine where the app will execute.
   You may want to do this when the application is installed:

   wevtutil im project.man
   NOTE:  must be an admin to do this!!!

2. Start the session for the application

   logman start -ets project -p "Provider Name From Manifest" 0 0 -o projectlog.etl
   NOTE:  must be an admin to do this!!!

3. Run the application:

   project.exe

4. Stop the logging session:

   logman stop -ets project  
   NOTE:  must be an admin to do this!!!

5. project.etl will contain the trace in binary format.  Convert it to text on the 
   machine where the .man file is installed.  i.e. the machine where the app ran
   or a separate local machine which also has the .man file installed:

   tracerpt -y project.etl            -- to generate an XML file report
   tracerpt -y -of CSV project.etl    -- to generate a CSV file report
   OR XPERF ????

   This produces an XML or CSV file which contains all the trace data for the application.

6. Remove the .man file as a provider when it's no longer needed, such as during
   application uninstall.

   wevtutil um project.man
   NOTE:  must be an admin to do this!!!

Resources:
http://blogs.microsoft.co.il/blogs/sasha/archive/2008/03/15/xperf-windows-performance-toolkit.aspx
http://blogs.msdn.com/pigscanfly/archive/2009/08/06/stack-walking-in-xperf.aspx
http://social.msdn.microsoft.com/Forums/en-US/etw/thread/a1aa1350-41a0-4490-9ae3-9b4520aeb9d4  (FAQ)
http://windowsclient.net/wpf/white-papers/event-tracing-wpf.aspx