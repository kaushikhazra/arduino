import sys
import urllib3
import requests

# If we have an invalid certification we will ignore and validate
urllib3.disable_warnings(urllib3.exceptions.InsecureRequestWarning)

# Request of XSRF token
auth = ('user3', 'accesscode::code::1234')
response = requests.get('https://192.168.1.10/api/myUser', verify=False,auth=auth)
print(response.json())
xsrf = None
for cookie in response.cookies:
    if cookie.name == 'XSRF-TOKEN':
        xsrf = cookie.value
        break

# Set the cookey
cookies = (cookie.name + '=' + cookie.value for cookie in response.cookies)
cookie_str = ';'.join(cookies)
print(response.headers)

headers = {'Cookie':cookie_str, 'X-XSRF-TOKEN': xsrf,'Content-Type': 'application/json','SiteId': '-1'}

# print(headers)
result = None 
while True:
    resp = requests.post('https://192.168.1.10/workflow/instructions', json=result, headers=headers, verify=False)
    resp_json = resp.json()
    
    id = resp_json['id']
    
    user_inputs = []
    for instruction in resp_json['instructions']:
        instruction_name =instruction['name']
        
        user_input = input(instruction['$type'] + ' ' + instruction['name'] + ' ' + instruction['prompt'] + ': ')
        
        if user_input.lower() == 'yes' : user_input = 'true'
        if user_input.lower() == 'no' : user_input = 'false'
        if user_input.lower() == 'exit' : sys.exit()
        
        user_inputs.append({
            'instructionName': instruction_name,
            'result': user_input,
            'isAdditionalCommand': False,
            'isScanned': False,
            'isScreenInput': False,
            'anchorWord': ''
        })
        
    result = { "id": id, "results": user_inputs}

    print(result)
    
