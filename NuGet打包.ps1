New-Item -Path .\nuget\build\native\include -ItemType Directory -Force
Copy-Item .\*.h .\nuget\build\native\include\
Copy-Item .\README.md .\nuget
nuget pack nuget