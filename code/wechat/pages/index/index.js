//index.js
//获取应用实例
const app = getApp()

var jishu = 0;

Page({
  data: {
    Startx:'',
    Starty: '',
    Endx: '',
    Endy: '',
    V: '',

    motto: 'Hello World',
    userInfo: {},
    hasUserInfo: false,
    canIUse: wx.canIUse('button.open-type.getUserInfo')
  },
  StartxInput: function (e) {
    this.setData({
      Startx: e.detail.value
    }),
      wx.showToast({
        title: '起点x:' + e.detail.value,
        duration: 1000
      })
  },
  StartyInput: function (e) {
    this.setData({
      Starty: e.detail.value
    }),
      wx.showToast({
        title: '起点y:' + e.detail.value,
        duration: 1000
      })
  },
  EndxInput: function (e) {
    this.setData({
      Endx: e.detail.value
    }),
      wx.showToast({
        title: '终点x:' + e.detail.value,
        duration: 1000
      })
  },
  EndyInput: function (e) {
    this.setData({
      Endy: e.detail.value
    }),
      wx.showToast({
        title: '终点y:' + e.detail.value,
        duration: 1000
      })
  },
  VInput: function (e) {
    this.setData({
      V: e.detail.value
    }),
      wx.showToast({
        title: '速度v:' + e.detail.value,
        duration: 1000
      })
  },



  //事件处理函数
  bindViewTap: function() {
    wx.navigateTo({
      url: '../logs/logs'
    })
  },
  onLoad: function () {
    if (app.globalData.userInfo) {
      this.setData({
        userInfo: app.globalData.userInfo,
        hasUserInfo: true
      })
    } else if (this.data.canIUse){
      // 由于 getUserInfo 是网络请求，可能会在 Page.onLoad 之后才返回
      // 所以此处加入 callback 以防止这种情况
      app.userInfoReadyCallback = res => {
        this.setData({
          userInfo: res.userInfo,
          hasUserInfo: true
        })
      }
    } else {
      // 在没有 open-type=getUserInfo 版本的兼容处理
      wx.getUserInfo({
        success: res => {
          app.globalData.userInfo = res.userInfo
          this.setData({
            userInfo: res.userInfo,
            hasUserInfo: true
          })
        }
      })
    }
  },
  getUserInfo: function(e) {
    console.log(e)
    app.globalData.userInfo = e.detail.userInfo
    this.setData({
      userInfo: e.detail.userInfo,
      hasUserInfo: true
    })
  },
  CreateBtnClick:function(e){
    console.warn("success");
    console.log(this.data.Startx + "," + this.data.Starty + "," + this.data.Endx + "," + this.data.Endy + "," + this.data.V);
    if(this.data.Startx == "" || this.data.Starty == "" || this.data.Endx == "" || this.data.Endy == "" || this.data.V == "" )
    {
      wx.showToast({
        title: '请输入完整信息',
        duration: 2000
      })
    }
    else
    {
      wx.request({
        //url: 'http://39.105.218.125:8888/test_app/',
        url: 'http://planesystem.xyz:8888/test_app/',
        data: {
          a0: '1',
          a1: this.data.Startx,
          a2: this.data.Starty,
          a3: this.data.Endx,
          a4: this.data.Endy,
          a5: this.data.V,
        },
        header: {
          "Content-Type": "application/x-www-form-urlencoded"
        },
        method: "GET",
        success: function (res) {
          jishu = jishu + 1;
          console.warn("success back!");
          console.log(jishu);
          console.log(res.data);

          if (res.data == '0') {
            wx.showToast({
              title: '数据库写入失败',
              duration: 2000
            })
          }
          else {
            wx.showModal({
              title: '尝试加入飞机',
              content: '第 ' + jishu.toString() + ' 架次',
            })
          }
        },
        fail: function (res) {
          wx.showToast({
            title: '服务器未运行',
            duration: 2000
          })
        }

      })


    }
  },

  Clean1Click:function(e){
    wx.request({
      url: 'http://planesystem.xyz:8888/test_app/',
      data: {
        a0: '2',
        a1: this.data.Startx,
        a2: this.data.Starty,
        a3: this.data.Endx,
        a4: this.data.Endy,
        a5: this.data.V,
      },
      header: {
        "Content-Type": "application/x-www-form-urlencoded"
      },
      method: "GET",
      success: function (res) {
        jishu = jishu + 1;
        console.warn("success back!");
        console.log(jishu);
        console.log(res.data);

        if (res.data == '0') {
          wx.showToast({
            title: '数据库写入失败',
            duration: 2000
          })
        }
        else {
          wx.showToast({
            title: '已清空user_add',
            duration: 2000
          })
        }
      },
      fail: function (res) {
        wx.showToast({
          title: '服务器未运行',
          duration: 2000
        })
      }

    })

  },

/*
  Clean2Click:function(e){
    wx.request({
      url: 'http://planesystem.xyz:8888/test_app/',
      data: {
        a0: '3',
        a1: this.data.Startx,
        a2: this.data.Starty,
        a3: this.data.Endx,
        a4: this.data.Endy,
        a5: this.data.V,
      },
      header: {
        "Content-Type": "application/x-www-form-urlencoded"
      },
      method: "GET",
      success: function (res) {
        jishu = jishu + 1;
        console.warn("success back!");
        console.log(jishu);
        console.log(res.data);

        if (res.data == '0') {
          wx.showToast({
            title: '数据库写入失败',
            duration: 2000
          })
        }
        else {
          wx.showToast({
            title: '已清空整个数据库',
            duration: 2000
          })
        }
      },
      fail: function (res) {
        wx.showToast({
          title: '服务器未运行',
          duration: 2000
        })
      }

    })

  }
*/


////
  Clean2Click: function (e) {
    wx.request({
      url: 'http://api.heclouds.com/devices/522297937/datapoints',
      header: {
        "api-key": 'FTI6Fa7TkNwH4uYGZfQ4H6sRpZ4='
      },
      method: "GET",
      success: function (res) {
        jishu = jishu + 1;
        console.warn("success back!");
        console.log(jishu);
        console.log(res.data.data.datastreams["0"].datapoints["0"].value);
        
        if (res.data == '0') {
          wx.showToast({
            title: '数据库写入失败',
            duration: 2000
          })
        }
        else {
          wx.showToast({
            title: "温度："+ res.data.data.datastreams["1"].datapoints["0"].value,
            duration: 2000
          })
        }
      },
      fail: function (res) {
        wx.showToast({
          title: '服务器未运行',
          duration: 2000
        })
      }

    })

  }
/////



})
