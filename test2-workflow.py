import urllib3
import requests

url = "https://192.168.1.10/api/myUser"

payload = {}
headers = {
  'Authorization': 'Basic dXNlcjM6YWNjZXNzY29kZTo6Y29kZTo6MTIzNA==',
  'Cookie': 'JSESSIONID=8F6E86281E9CC44D697DCAD5344FF735; XSRF-TOKEN=53670fc5-727b-4eed-8a6a-6714461061f5'
}

urllib3.disable_warnings(urllib3.exceptions.InsecureRequestWarning)
response = requests.get(url, verify=False, auth = ('user3', 'accesscode::code::1234'))

print(response)
