import os,base64
env_dist = os.environ
size =  int(env_dist.get('Content-Length'))
param_b = os.read(0, size)
param_s = bytes.decode(param_b)


#while True:
#    strs = input()
#    if strs == '':
#        break
#    string += strs
name = param_s.split('"', 3)[1]

image_name = './httproot/image/'+ name + '.png'
b64str_ = param_s.split(',')[1]
b64str = b64str_.split('\n')[0]

#str_size = str(size)
#with open('./httproot/image/image.txt', 'w') as f:
#    f.write(b64str1)

imgdata=base64.b64decode(b64str)
file=open(image_name, 'wb')
file.write(imgdata)
file.close() 
print('1')
