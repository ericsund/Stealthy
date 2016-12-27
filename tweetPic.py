import sys, os, tweepy, glob, time
from twython import Twython
from datetime import datetime

#Setup the API
consumer_key = 'BSXME3bXoJ8WaXpMRgZqJr3cc'
consumer_secret = 'o4vqgxCzQicyzc3KJMEt6pwKvSk05CkIgcOMUo8NonXlZh3wPF'
access_key = '1560989455-C0ns18sGyWNnC3zpByWRK3rCnLuo8Jh0sgnJST5'
access_secret= 'iCSjZJd0ePbc2PPT8sz3IKBllELCsIi704o9SQYNZdQri'

t = Twython(consumer_key, consumer_secret, access_key, access_secret)

print("[NOTICE] Now monitoring for new photos to tweet!")

while True:
    #Find the picture that was just taken
    try: #new picture exists... so get its name
        getNewestPhoto = max(glob.iglob('*.jpg'), key=os.path.getctime)
        print("[NOTICE] Found new photo!")

        #open it
        newestPhoto = open(getNewestPhoto, "r") #open it

        #Get the current date and time
        currentDateTime = datetime.now().strftime("%Y-%m-%d | %H:%M:%S")

        #Tweet out the photo with a date and time caption
        t.update_status_with_media(media = newestPhoto, status = currentDateTime)

        #wait a bit and delete the photo that was just tweeted
        os.system("rm " + str(getNewestPhoto))

    except ValueError: #new picture doesn't exist yet... so wait a bit
        print("[NOTICE] Didn't find new photo.  Trying again.")
