#
# Be sure to run `pod lib lint TPWBFaceVerify.podspec' to ensure this is a
# valid spec before submitting.
#
# Any lines starting with a # are optional, but their use is encouraged
# To learn more about a Podspec see http://guides.cocoapods.org/syntax/podspec.html
#

Pod::Spec.new do |s|
s.name             = 'TPWBFaceVerify'
s.version          = '0.0.1'
s.summary          = 'Face recognition for ios.'

# This description is used to generate tags and improve search results.
#   * Think: What does it do? Why did you write it? What is the focus?
#   * Try to keep it short, snappy and to the point.
#   * Write the description between the DESC delimiters below.
#   * Finally, don't worry about the indent, CocoaPods strips it!

s.description      = <<-DESC
TODO: Add long description of the pod here.
DESC

s.homepage         = 'https://github.com/471289774/TPWBFaceVerify'
# s.screenshots     = 'www.example.com/screenshots_1', 'www.example.com/screenshots_2'
s.license          = { :type => 'Apache License, Version 2.0', :text => <<-LICENSE
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
LICENSE
}
s.author           = { '顾卫明' => '471289774@qq.com' }
s.source           = { :git => 'https://github.com/471289774/TPWBFaceVerify.git'}
# s.social_media_url = 'https://twitter.com/<TWITTER_USERNAME>'

s.ios.deployment_target = '8.0'

s.source_files = 'TPWBFaceVerify/Classes/**/*'

# s.resource_bundles = {
#   'TPWBFaceVerify' => ['TPWBFaceVerify/Assets/*.png']
# }

# s.public_header_files = 'Pod/Classes/**/*.h'
s.frameworks = 'CoreMedia', 'AudioToolbox','AVFoundation','CoreTelephony','GLKit','Accelerate','MobileCoreServices','SystemConfiguration'
s.vendored_frameworks = 'TPWBFaceVerify/Assets/NextCV.framework','TPWBFaceVerify/Assets/WBCloudFaceVerifySDK.framework','TPWBFaceVerify/Assets/YTFaceSDK.framework'
s.libraries = 'c++'
s.vendored_libraries = 'TPWBFaceVerify/Assets/libcrypto.a','TPWBFaceVerify/Assets/libssl.a'
s.resources = ['TPWBFaceVerify/Assets/ufa.bundle','TPWBFaceVerify/Assets/WBFaceV2Pics.bundle','TPWBFaceVerify/Assets/youtubeauty.bundle']
end
