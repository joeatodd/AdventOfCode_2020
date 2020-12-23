* Day 20 - Sea monsters!

Constructing a large image from rotated & flipped sub-images by matching edges. I tried to do this OO, which worked but probably took me longer than it would have. Learned a fair bit about c++ though.

This is also way more verbose than it needs to be, because I switched half way through from storing the 2D grids as a 1D array w/ implicit dims to storing a 2D ```vector<vector<char>>``` instead. This meant I had to write separate routines for flipping & rotating the sub-images and the full image.

** Part 2

Once the full image is constructed, the task for Part 2 is to find all the sea monsters:

```
                  # 
#    ##    ##    ###
 #  #  #  #  #  #   
```

This is fairly easy - just scan across the image in every orientation.